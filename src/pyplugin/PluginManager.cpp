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

#include "PluginManager.h"
#include <RuntimeConfig.h>

using namespace boost::python;

PluginManager* PluginManager::Instance()
{
    static PluginManager* instance = nullptr;
    if(instance == nullptr)
    {
        instance = new PluginManager();
    }
    return instance;
}

void PluginManager::DestroyInstance()
{
    L_INFO("Destroying managed plugins...");
    delete PluginManager::Instance();
}

void PluginManager::RegisterPlugin(PluginRef plugin)
{
    PluginManager::Instance()->registerPlugin(plugin);
}

PluginManager::PluginManager()
{
    
}

void PluginManager::registerPlugin(PluginRef plugin)
{
    // making sure the plugin hook exists
    QString hook = QString::fromStdString(plugin->getHook());
    if(RuntimeConfig::Instance()->AvailableHooks().contains(hook))
    {   
        // checking if we already have a list registered to this hook
        if(!m_mapper.contains(hook))
        {
            m_mapper[hook] = PluginList();
        }
        
        // @TODO Add extra validation to ensure plugin has been set up correctly.
        
        // sorting plugins in order...
        PluginList &pluglist = m_mapper[hook];
        PluginList::iterator iter = pluglist.begin();
        while(iter != pluglist.end())
        {
            if(plugin->getOrder() >= (*iter)->getOrder())
            {
                break;
            }
            ++iter;
        }
        pluglist.insert(iter, plugin);
        L_INFO(QString("Registered plugin: %1").arg(QString::fromStdString(plugin->getName())));
    }
    else
    {
        L_ERROR(
            QString("Failed to register plugin \"%1\" for reason: Empty or invalid hook identifier. Hook: \"%2\"")
            .arg(QString::fromStdString(plugin->getName()), hook)
        );
    }
}

void PluginManager::executePluginsForHook(QString hook, dict args, CurrentPluginUpdatedCallback updateCB, FinishedExecutingPluginCallback finishedCB)
{
    PluginList plugins = getPluginsForHook(hook);
    if(plugins.empty())
    {
        L_WARN(QString("No plugins exist for this hook: %1").arg(hook));
        return;
    }

    for(int i = 0; i < plugins.length(); ++i)
    {
        if(updateCB) updateCB(plugins[i], i, plugins.length());
        if(!plugins[i]->execHook(args))
        {
            if(finishedCB) finishedCB(false);
            return;
        }
    }

    if(finishedCB) finishedCB(true);
}

PluginList PluginManager::getPluginsForHook(QString hook)
{
    if(m_mapper.contains(hook))
    {
        return m_mapper[hook];
    }
    return PluginList();
}