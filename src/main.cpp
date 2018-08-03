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
#include "RuntimeConfig.h"
#include <pyplugin/PyLogger.h>

#if PY_MAJOR_VERSION >= 3
#   define INIT_MCMODINTERNAL_MODULE PyInit_mcmodinternal
extern "C" PyObject* INIT_MCMODINTERNAL_MODULE();
#else
#   define INIT_MCMODINTERNAL_MODULE initmcmodinternal
extern "C" void INIT_MCMODINTERNAL_MODULE();
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
    
    PyImport_AppendInittab("mcmodinternal", INIT_MCMODINTERNAL_MODULE);
    Py_SetProgramName(pypathc);
    Py_Initialize();
    
    // adding current directory to module...
    L_DEBUG("Adding scripts to interpreter path...");
    namespace bp = boost::python;
    try
    {
        bp::object sys = bp::import("sys");
        sys.attr("path").attr("append")(std::string(pypathc));
    }
    catch(bp::error_already_set &)
    {
        PyErr_Print();
        PyErr_Clear();
    }
    
    // running application
    L_INFO("Initializing MCModCrafter...");
    MCModCrafter w;
    w.show();
    return a.exec();
    
#if DEBUG
    Py_Main(argc, argv);
#endif // DEBUG
    
    Py_Finalize();
    delete[] pypathc;
}

#undef LOG_FMT
