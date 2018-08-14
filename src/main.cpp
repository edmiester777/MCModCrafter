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

#include "stdafx.h"
#include "ui/WindowMCModCrafter.h"
#include <QtWidgets/QApplication>
#include <QMutex>
#include <QWaitCondition>
#include "RuntimeConfig.h"
#include <pyplugin/PyLogger.h>
#include <pyplugin/PluginManager.h>

#if PY_MAJOR_VERSION >= 3
#   define INIT_MCMOD_MODULE PyInit_mcmod
extern "C" PyObject* INIT_MCMOD_MODULE();
#else
#   define INIT_MCMOD_MODULE initmcmod
extern "C" void INIT_MCMOD_MODULE();
#endif

void CreateDirIfNotExists(QString dir)
{
    QDir d(dir);
    if(!d.exists())
    {
        d.mkpath(".");
    }
}

int main(int argc, char *argv[])
{
    qRegisterMetaType<PluginRef>("PluginRef");
    QApplication a(argc, argv);
    
    // initializing logger...
    PyLogger::Init();

    L_INFO("Initializing python interpreter...");
    QString pypath = QDir::currentPath() + "/pylibs";
    char* pypathc = strdup(pypath.toStdString().c_str());
    
    // creating necessary directories
    CreateDirIfNotExists(RuntimeConfig::Instance()->GetLogsDirectory());
    CreateDirIfNotExists(RuntimeConfig::Instance()->GetProjectsDirectory());
    CreateDirIfNotExists(RuntimeConfig::Instance()->GetDownloadsDirectory());
    
    PyImport_AppendInittab("mcmod", INIT_MCMOD_MODULE);
    Py_SetProgramName(pypathc);
    PluginManager::Instance(); // forcing the loading and starting of python thread.
    
    // adding current directory to module...
    L_DEBUG("Adding scripts to interpreter path...");
    QMutex mtx;
    QWaitCondition condition;
    namespace bp = boost::python;
    PluginManager::Instance()->exec([&](){
        bp::object sys = bp::import("sys");
        sys.attr("path").attr("append")(std::string(pypathc));

        // attempting to load plugins
        try
        {
            bp::import("pluginscan");
        }
        catch(bp::error_already_set &)
        {
            PyErr_Print();
            PyErr_Clear();
            L_WARN("Failed to load plugins.");
        }

#ifdef DEBUG
        L_DEBUG("Starting interactive interpreter...");
        Py_Main(argc, argv);
#endif // DEBUG

        condition.wakeAll();
    });

    // waiting for plugin initialization to finish.
    mtx.lock();
    condition.wait(&mtx);
    mtx.unlock();

    // running application
    L_INFO("Initializing MCModCrafter...");
    MCModCrafter::Instance()->show();

    int res = a.exec();
    
    PluginManager::Instance()->exec([&](){
        delete MCModCrafter::Instance();
        PluginManager::Instance()->cleanupPlugins();
        
        condition.wakeAll();
    });
    
    // waiting for destruction to finish
    mtx.lock();
    condition.wait(&mtx);
    mtx.unlock();
    PluginManager::DestroyInstance();
    delete[] pypathc;
    
    return res;
}

#undef LOG_FMT
