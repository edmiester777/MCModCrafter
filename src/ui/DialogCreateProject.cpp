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
#include <QMessageBox>
#include "FileDownloader.h"

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
        accept();
    }
}

ProjectConfig& DialogCreateProject::Config()
{
    return m_config;
}

void DialogCreateProject::SetupNewProject()
{
    FileDownloader downloader;
}
