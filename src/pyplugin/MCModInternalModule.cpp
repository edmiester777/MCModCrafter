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
#include "PyLogger.h"
#include "PyPlugin.h"
#include "PluginManager.h"

BOOST_PYTHON_MODULE(mcmod)
{
    using namespace std;
    namespace bp = boost::python;
    
    // exporting the bridge logger
    bp::class_<PyLogger>("Logger", bp::init<string>())
    .def("debug", &PyLogger::Debug)
    .def("info", &PyLogger::Info)
    .def("warn", &PyLogger::Warn)
    .def("error", &PyLogger::Error)
    .def("fatal", &PyLogger::Fatal);
    
    bp::class_<PyPluginWrapper, boost::noncopyable>("Plugin", bp::init<string>())
    .def(bp::init<string, int>())
    .add_property("name", &PyPluginWrapper::getName, &PyPluginWrapper::setName)
    .add_property("description", &PyPluginWrapper::getDescription, &PyPluginWrapper::setDescription)
    .add_property("author", &PyPluginWrapper::getAuthor, &PyPluginWrapper::setAuthor)
    .add_property("version", &PyPluginWrapper::getVersion, &PyPluginWrapper::setVersion)
    .add_property("status_text", &PyPluginWrapper::getStatusText, &PyPluginWrapper::setStatusText)
    .add_property("logger", &PyPluginWrapper::getLogger)
    .def("exec_hook", &PyPluginWrapper::execHook);
    
    bp::def("register_plugin", &PluginManager::RegisterPlugin);
}
