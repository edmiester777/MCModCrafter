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
#include "WindowMCModCrafter.h"
#include "WidgetSelectProject.h"

MCModCrafter::MCModCrafter(QWidget *parent)
    : QMainWindow(parent)
{
    m_ui.setupUi(this);
    m_content = nullptr;

    SetContent(new WidgetSelectProject);
}

void MCModCrafter::SetContent(QWidget* content)
{
    if(m_content != nullptr)
    {
        delete m_content;
    }
    m_content = content;
    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(m_content);
    m_ui.centralWidget->setLayout(layout);
}
