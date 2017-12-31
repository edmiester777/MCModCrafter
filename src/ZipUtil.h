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

#ifndef __FILE_DOWNLOADER_H__
#define __FILE_DOWNLOADER_H__

#include "stdafx.h"

/**
 * @brief Zip file utilities.
 *
 * This class contains useful functions that pertain to zip files.
 */
class ZipUtil
{
public:
    /**
     * @brief Unzip a file to a directory.
     *
     * This function uses the python runtime to unzip files to destination
     * folders. It was much easier just to exploit the python interpreter
     * and standard library for this functionality.
     *
     * @param src Absolute path to source file.
     * @param dest Absolute path to the destination folder.
     *
     * @return Success
     */
    static bool UnzipFile(QString src, QString dest);
};

#endif //!__FILE_DOWNLOADER_H__