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

#include <QtCore>
#include <QtWidgets>
#include <QJsonDocument>
#include <QDir>
#include <QFile>
#include "simpleQtLogger.h"
#include <boost/python.hpp>
#include <pyplugin/AcquireGIL.h>

#define HOOK_CREATEPROJECT "mcmod.createproject.setup"

using namespace simpleqtlogger;
