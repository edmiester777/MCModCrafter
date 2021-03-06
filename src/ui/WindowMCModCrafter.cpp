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

#include <stdafx.h>
#include "WindowMCModCrafter.h"
#include "WidgetSelectProject.h"

// REMOVE ME
#include "WidgetProjectSetup.h"

MCModCrafter* MCModCrafter::Instance()
{
    static MCModCrafter* instance = nullptr;
    if(instance == nullptr)
        instance = new MCModCrafter;
    return instance;
}

MCModCrafter::MCModCrafter(QWidget *parent)
    : QMainWindow(parent)
{
    m_ui.setupUi(this);
    m_content = nullptr;

    SetContent(new WidgetSelectProject);
    connect(
            PluginManager::Instance(),
            SIGNAL(ExecutingHook(QString, PluginRef, int, int)),
            this,
            SLOT(PluginExecuted(QString, PluginRef, int, int))
    );
    connect(
        PluginManager::Instance(),
        SIGNAL(ExecutingFinished(bool)),
        this,
        SLOT(PluginExecutionFinished(bool))
    );

    // executing the init script...
    PluginManager::Instance()->exec(PluginManager::Instance()->executePluginsForHook("mcmod.init"));
}

void MCModCrafter::SetContent(QWidget* content)
{
    if(m_content != nullptr)
    {
        delete m_content;
    }
    m_content = content;
    delete m_ui.centralWidget->layout();
    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(m_content);
    m_ui.centralWidget->setLayout(layout);
}

void MCModCrafter::PluginExecuted(QString hook, PluginRef plugin, int current, int total)
{
    QString format = "%1 (%2/%3)...";
    QString message = format.arg(
        QString::fromStdString(plugin->getStatusText()),
        QString::number(current + 1),
        QString::number(total)
    );
    statusBar()->showMessage(message);
}

void MCModCrafter::PluginExecutionFinished(bool success)
{
    if(!success)
        statusBar()->showMessage("One or more errors occurred while executing hook.");
    else
        statusBar()->showMessage("");
}
