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
#include "WidgetSelectProject.h"
#include "DialogCreateProject.h"
#include "RuntimeConfig.h"
#include <pyplugin/PluginManager.h>
#include <QDir>
#include <QMessageBox>

WidgetSelectProject::WidgetSelectProject(QWidget *parent)
    : QWidget(parent)
{
    m_ui.setupUi(this);

    connect(m_ui.createProjectButton, SIGNAL(clicked()), this, SLOT(CreateProjectButtonClicked()));
}

void WidgetSelectProject::CreateProjectButtonClicked()
{
    L_INFO("Opening create project dialog...");
    DialogCreateProject dcp;
    dcp.exec();
}
