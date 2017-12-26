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

#ifndef __DIALOGCREATEPROJECT_H__
#define __DIALOGCREATEPROJECT_H__

#include <QDialog>
#include <ui_DialogCreateProject.h>
#include "ProjectConfig.h"

class DialogCreateProject : public QDialog
{
    Q_OBJECT
public:
    DialogCreateProject(QWidget *parent = Q_NULLPTR);

    /**
     * Get the config that was generated using this dialog.
     *
     * @return Config generated if this dialog was accept()ed.
     */
    ProjectConfig& Config();

private slots:
    void ValidateInfo();

private:
    Ui::DialogCreateProject m_ui;
    ProjectConfig m_config;
};

#endif //!__DIALOGCREATEPROJECT_H__
