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

#ifndef __WINDOWMCMODCRAFTER_H__
#define __WINDOWMCMODCRAFTER_H__

#include <QtWidgets/QMainWindow>
#include <ui_WindowMCModCrafter.h>

class MCModCrafter : public QMainWindow
{
    Q_OBJECT
public:
    MCModCrafter(QWidget *parent = Q_NULLPTR);

    void SetContent(QWidget* content);

private:
    Ui::WindowMCModCrafter m_ui;
    QWidget* m_content;
};

#endif //!__WINDOWMCMODCRAFTER_H__
