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
#include "WindowMCModCrafter.h"
#include <QtWidgets/QApplication>
#include "RuntimeConfig.h"

#define MB10 1024*1024*10

int main(int argc, char *argv[])
{
    // configuring logger
    simpleqtlogger::ENABLE_LOG_SINK_FILE = true;
    simpleqtlogger::ENABLE_LOG_SINK_CONSOLE = true;
    simpleqtlogger::ENABLE_LOG_SINK_QDEBUG = false;
    simpleqtlogger::ENABLE_LOG_SINK_SIGNAL = true;
    // set log-features
    simpleqtlogger::ENABLE_FUNCTION_STACK_TRACE = true;
    simpleqtlogger::ENABLE_CONSOLE_COLOR = true;
    // set log-levels (global; all enabled)
    simpleqtlogger::ENABLE_LOG_LEVELS.logLevel_DEBUG = true;
    simpleqtlogger::ENABLE_LOG_LEVELS.logLevel_FUNCTION = true;
    // set log-levels (specific)
    simpleqtlogger::EnableLogLevels enableLogLevels_file = simpleqtlogger::ENABLE_LOG_LEVELS;
    simpleqtlogger::EnableLogLevels enableLogLevels_console = simpleqtlogger::ENABLE_LOG_LEVELS;
    simpleqtlogger::EnableLogLevels enableLogLevels_qDebug = simpleqtlogger::ENABLE_LOG_LEVELS;
    simpleqtlogger::EnableLogLevels enableLogLevels_signal = simpleqtlogger::ENABLE_LOG_LEVELS;
    enableLogLevels_console.logLevel_FUNCTION = false;
    simpleqtlogger::EnableLogLevels enableLogLevels_fileWarn = simpleqtlogger::ENABLE_LOG_LEVELS;
    enableLogLevels_fileWarn.logLevel_NOTE = false;
    enableLogLevels_fileWarn.logLevel_INFO = false;
    enableLogLevels_fileWarn.logLevel_DEBUG = false;
    enableLogLevels_fileWarn.logLevel_FUNCTION = false;

    // constructing logger
    SimpleQtLogger::createInstance(nullptr)->setLogFileName("logs/MCModCrafter.log", MB10, 50);

    // running application
    QApplication a(argc, argv);
    MCModCrafter w;
    w.show();
    return a.exec();
}
