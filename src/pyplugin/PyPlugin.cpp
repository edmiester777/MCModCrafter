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

#include "PyPlugin.h"

PyPlugin::PyPlugin(string hook, int order)
{
    
    m_name = "unknown";
    m_logger = PyLogger(m_name);
}

// ACCESSORS / MUTATORS
void PyPlugin::setName(string name)
{
    m_name = name;
    m_logger = PyLogger(m_name);
}

string PyPlugin::getName()
{
    return m_name;
}

void PyPlugin::setDescription(std::string description)
{
    m_description = description;
}

string PyPlugin::getDescription()
{
    return m_description;
}

void PyPlugin::setAuthor(std::string author)
{
    m_author = author;
}

string PyPlugin::getAuthor()
{
    return m_author;
}

void PyPlugin::setVersion(std::string version)
{
    m_version = version;
}

string PyPlugin::getVersion()
{
    return m_version;
}

void PyPlugin::setStatusText(string text)
{
    m_statusText = text;
}

string PyPlugin::getStatusText()
{
    return m_statusText;
}

void PyPlugin::setHook(string hook)
{
    // @TODO Add validation for existing hooks
    m_hook = hook;
}

string PyPlugin::getHook()
{
    return m_hook;
}

void PyPlugin::setOrder(int order)
{
    m_order = order;
}

int PyPlugin::getOrder()
{
    return m_order;
}

PyLogger PyPlugin::getLogger()
{
    return m_logger;
}

// PLUGIN OVERRIDES
bool PyPlugin::execHook()
{
    getLogger().Warn("Executing default exec_hook() method... Did you forget to override this?");
    return false;
}
