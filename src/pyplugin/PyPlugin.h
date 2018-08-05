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

#ifndef __PYPLUGIN_H__
#define __PYPLUGIN_H__

#include <stdafx.h>
#include "PyLogger.h"

#define DEFAULT_PLUGIN_ORDER 10

using namespace std;
using namespace boost::python;

/**
 * @brief Plugin structure for a python plugin.
 *
 * This class contains the logic and structure for a python plugin into this
 * application. I want to be able to maintain hooks to the application as well
 * as extended functionality.
 */
class PyPlugin
{
public:
    PyPlugin(string hook, int order = DEFAULT_PLUGIN_ORDER);
    virtual ~PyPlugin() = default;
    
    // ACCESSORS / MUTATORS
    void setName(string name);
    string getName();
    void setDescription(string description);
    string getDescription();
    void setAuthor(string author);
    string getAuthor();
    void setVersion(string version);
    string getVersion();
    void setStatusText(string text);
    string getStatusText();
    void setHook(string hook);
    string getHook();
    void setOrder(int order);
    int getOrder();
    PyLogger getLogger();
    
    /**
     * This is the plugin functionality that will be executed.
     *
     * @return True if execution of other plugins should continue, else false.
     */
    virtual bool execHook();
    
private:
    PyLogger m_logger;
    string m_name;
    string m_description;
    string m_author;
    string m_version;
    string m_statusText;
    string m_hook;
    int m_order;
};

class PyPluginWrapper : public PyPlugin, public wrapper<PyPlugin>
{
public:
    PyPluginWrapper(string hook, int order = DEFAULT_PLUGIN_ORDER) : PyPlugin(hook, order) {}
    virtual bool execHook() override
    {
        try
        {
            if(override func = get_override("exec_hook"))
            {
                getLogger().Info((char *)"Calling exec_hook()...");
                return func();
            }
            return PyPlugin::execHook();
        }
        catch(error_already_set &)
        {
            PyErr_Print();
            getLogger().Error("Failed to execute method \"exec_hook()\".");
        }
        return false;
    }
};

#undef DEFAULT_PLUGIN_ORDER
#endif // !__PYPLUGIN_H__
