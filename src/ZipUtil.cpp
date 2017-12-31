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

#include "ZipUtil.h"

bool ZipUtil::UnzipFile(QString src, QString dest)
{
    using namespace boost::python;
    try
    {
        L_DEBUG(
            QString("Unzipping file %1 to %2 using python interp...")
            .arg(src, dest)
        );
        object zipfilemodule = import("zipfile");
        object zipfileclass = zipfilemodule.attr("ZipFile");
        str pysrc(src.toStdString().c_str());
        str pydest(dest.toStdString().c_str());
        object zipfile = zipfileclass(pysrc, str("r"));
        object extractFunc = zipfile.attr("extractall");
        object closeFunc = zipfile.attr("close");
        call<void>(extractFunc.ptr(), pydest);
        call<void>(closeFunc.ptr());
        L_DEBUG("Done extracting!");
        return true;
    }
    catch(std::exception& ex)
    {
        Q_UNUSED(ex);
        L_ERROR(
            QString("Unzip with params (%1, %2) has triggered following error:")
            .arg(src, dest)
        );
        PyErr_Print();
        PyErr_Clear();
    }
    return false;
}