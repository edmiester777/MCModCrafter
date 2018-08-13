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
    if(dcp.exec() == QDialog::Accepted)
    {
        ProjectConfig config;
        config = dcp.Config();
        QDir projDir(RuntimeConfig::Instance()->GetProjectsDirectory());
        QDir subprojDir(projDir.absolutePath() + QDir::separator() + config.GetProjectName());
        if(subprojDir.exists())
        {
            L_WARN("User tried to create a project that already exists.");
            QMessageBox mb(
                QMessageBox::Icon::Information,
                "Error",
                QString("You already have a project named \"%1\"!").arg(config.GetProjectName()),
                QMessageBox::Button::Ok
            );
            mb.exec();
        }
        else
        {
            subprojDir.mkpath(".");
            bool success = config.Save(subprojDir.absoluteFilePath(RuntimeConfig::Instance()->GetProjectConfigName()));
            if(!success)
            {
                QMessageBox mb(
                    QMessageBox::Icon::Critical,
                    "Error",
                    "Could not save project configuration.",
                    QMessageBox::Button::Ok
                );
                mb.exec();
                return;
            }
            using namespace boost::python;
            dict kwargs;
            kwargs["dir"] = subprojDir.absolutePath().toStdString();
            PluginManager::Instance()->executePluginsForHook("mcmod.createproject.setup", kwargs, nullptr, [&](bool finishedSuccessfully){
                success = finishedSuccessfully;
            });
        }
    }
}
