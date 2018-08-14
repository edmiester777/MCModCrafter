/*************************************************************************
* MCModCrafter Program to help introduce people to minecraft modding and
* to speed development time via logic visualization.
*
* Copyright (C) 2017 Edward Callahan
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program.  If not, see <https://www.gnu.org/licenses/>.
*************************************************************************/

#include "WidgetProjectSetup.h"
#include <stdafx.h>
#include <QDir>
#include <QMessageBox>
#include <QVBoxLayout>
#include <pyplugin/PluginManager.h>

WidgetProjectSetup::WidgetProjectSetup(QDir path, QWidget *parent)
{
    m_path = path;
    m_ui.setupUi(this);
    if(path.exists())
        path.removeRecursively();
    setupPlugins();
}

void WidgetProjectSetup::beginInstall()
{
    using namespace boost::python;
    
    dict kwargs;
    kwargs["dir"] = m_path.absolutePath().toStdString();
    
    PluginHookThreadWorker *worker = PluginManager::Instance()->executePluginsForHook(HOOK_CREATEPROJECT, kwargs);
    
    connect(
        worker,
        SIGNAL(nextPluginStarted(PluginRef, int, int)),
        this,
        SLOT(nextPlugin(PluginRef, int, int))
    );
    connect(
        worker,
        SIGNAL(finishedExecuting(bool, PluginRef, int)),
        this,
        SLOT(finishedHook(bool, PluginRef, int))
    );
    
    PluginManager::Instance()->exec(worker);
}

void WidgetProjectSetup::setupPlugins()
{
    PluginList plugins = PluginManager::Instance()->getPluginsForHook(HOOK_CREATEPROJECT);
    QVBoxLayout *vlayout = new QVBoxLayout;
    for(PluginList::iterator iter = plugins.begin(); iter != plugins.end(); ++iter)
    {
        WidgetPluginStatus *plugui = new WidgetPluginStatus(*iter);
        vlayout->addWidget(plugui);
        m_pluginWidgets.push_back(plugui);
    }
    m_ui.centerWidget->setLayout(vlayout);
}

void WidgetProjectSetup::nextPlugin(PluginRef plugin, int index, int total)
{
    m_ui.progressBar->setValue((int)(((double)index / total) * 100));
    if(index > 0)
        m_pluginWidgets[index - 1]->setSuccess();
    m_pluginWidgets[index]->setRunning();
}

void WidgetProjectSetup::finishedHook(bool success, PluginRef failedPlugin, int failedIndex)
{
    if(!success)
    {
        for(int i = failedIndex; i < m_pluginWidgets.length(); ++i)
        {
            m_pluginWidgets[i]->setError();
        }
    }
    else
    {
        m_pluginWidgets[m_pluginWidgets.length() - 1]->setSuccess();
        m_ui.progressBar->setValue(100);
    }
}
