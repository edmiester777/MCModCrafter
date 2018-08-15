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
#include "ProjectConfig.h"
#include <pyplugin/PluginManager.h>
#include <QDir>
#include <QDirIterator>
#include <QMessageBox>

WidgetSelectProject::WidgetSelectProject(QWidget *parent)
    : QWidget(parent)
{
    m_ui.setupUi(this);

    connect(m_ui.createProjectButton, SIGNAL(clicked()), this, SLOT(createProjectButtonClicked()));
    
    scanForProjects();
}

void WidgetSelectProject::createProjectButtonClicked()
{
    L_INFO("Opening create project dialog...");
    DialogCreateProject dcp;
    dcp.exec();
}

void WidgetSelectProject::selectProjectClicked(QString dir)
{
    L_DEBUG("Opening project at \"" + dir + "\"");
}

void WidgetSelectProject::scanForProjects()
{
    L_DEBUG("Scanning for projects...");
    QDir projectsdir("./" + RuntimeConfig::Instance()->GetProjectsDirectory());
    QString projDirPath = projectsdir.absolutePath();
    QDirIterator iter(projDirPath, QDir::Dirs | QDir::NoDotAndDotDot, QDirIterator::Subdirectories);
    while(iter.hasNext())
    {
        iter.next();
        QString projectFileName = iter.filePath() + "/" + RuntimeConfig::Instance()->GetProjectConfigName();
        L_DEBUG("Scanning \"" + iter.filePath() + "\" for projects...");
        if(QFile::exists(projectFileName))
        {
            L_DEBUG("Found project at \"" + iter.filePath() + "\"");
            ProjectConfig config;
            config.Load(projectFileName);
            
            QPushButton *button = new QPushButton;
            button->setText(config.GetProjectName());
            QString path = iter.filePath();
            connect(button, &QPushButton::clicked, [=](){
                selectProjectClicked(path);
            });
            
            m_ui.buttonsList->layout()->addWidget(button);
        }
    }
    L_DEBUG("Done scanning for projects!");
}
