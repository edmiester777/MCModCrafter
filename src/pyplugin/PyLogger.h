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
#ifndef __PYLOGGER_H__
#define __PYLOGGER_H__

#include <stdafx.h>

/**
 * @brief Python bridge for logging.
 *
 * This class will allow python to execute in the same way for both C++ and
 * python. This bridges the gap allowing us to use the same locking mechanism
 * for both python and C++. Well, this is just the simplest way I could think
 * to do it.
 */
class PyLogger
{
public:
    /**
     * @brief Initialize the internal logger.
     *
     * This will take care of all the initial setup for the logger.
     * which includes instantiation.
     */
    static void Init();
    
    /**
     * Log a debug statement
     *
     * @param text Text to log
     */
    static void Debug(const char *text);
    
    /**
     * Log a debug statement
     *
     * @param text Text to log
     */
    static void Info(const char *text);
    
    /**
     * Log a debug statement
     *
     * @param text Text to log
     */
    static void Warn(const char *text);
    
    /**
     * Log a debug statement
     *
     * @param text Text to log
     */
    static void Error(const char *text);
    
    /**
     * Log a debug statement
     *
     * @param text Text to log
     */
    static void Fatal(const char *text);
};

#endif // !__PYLOGGER_H__
