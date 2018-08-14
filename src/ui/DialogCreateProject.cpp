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
#include "DialogCreateProject.h"
#include "WidgetProjectSetup.h"
#include "WindowMCModCrafter.h"
#include <QMessageBox>
#include <QDir>
#include <RuntimeConfig.h>

DialogCreateProject::DialogCreateProject(QWidget *parent)
    : QDialog(parent)
{
    setWindowTitle("Create Project");

    m_ui.setupUi(this);
    
    connect(m_ui.dialogButtons, SIGNAL(accepted()), this, SLOT(ValidateInfo()));
    connect(m_ui.dialogButtons, SIGNAL(rejected()), this, SLOT(reject()));
}

void DialogCreateProject::ValidateInfo()
{
    bool accepted =
        !m_ui.projectNameLineEdit->text().isEmpty() &&
        !m_ui.projectDescriptionLineEdit->text().isEmpty() &&
        !m_ui.projectModIDLineEdit->text().isEmpty() &&
        !m_ui.projectAuthorLineEdit->text().isEmpty() &&
        !m_ui.projectVersionLineEdit->text().isEmpty();
    if(!accepted)
    {
        QMessageBox mbox(
            QMessageBox::Icon::Warning,
            "Error",
            "Please fill out all fields before creating project.",
            QMessageBox::Button::Ok
        );
        mbox.exec();
    }
    else
    {
        m_config = ProjectConfig();
        m_config.SetProjectName(m_ui.projectNameLineEdit->text());
        m_config.SetProjectDescription(m_ui.projectDescriptionLineEdit->text());
        m_config.SetProjectModID(m_ui.projectModIDLineEdit->text());
        m_config.SetProjectAuthor(m_ui.projectAuthorLineEdit->text());
        m_config.SetProjectVersion(m_ui.projectVersionLineEdit->text());
        SetupNewProject();
    }
}

ProjectConfig& DialogCreateProject::Config()
{
    return m_config;
}

void DialogCreateProject::SetupNewProject()
{
    ProjectConfig config;
    config = Config();
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
        WidgetProjectSetup *setupwidg = new WidgetProjectSetup(subprojDir);
        MCModCrafter::Instance()->SetContent(setupwidg);
        setupwidg->beginInstall();
        close();
    }
}
