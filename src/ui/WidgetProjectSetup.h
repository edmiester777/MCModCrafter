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

#ifndef __WIDGET_PROJECT_SETUP_H__
#define __WIDGET_PROJECT_SETUP_H__

#include <QWidget>
#include <ui_WidgetProjectSetup.h>
#include <QDir>
#include "WidgetPluginStatus.h"

using namespace std;

class WidgetProjectSetup : public QWidget
{
    Q_OBJECT
public:
    WidgetProjectSetup(QDir path, QWidget *parent = Q_NULLPTR);
    
    void beginInstall();
    
public slots:
    void nextPlugin(PluginRef plugin, int index, int total);
    void finishedHook(bool success, PluginRef failedPlugin, int failedIndex);

private:
    void setupPlugins();
    
    Ui::WidgetProjectSetup m_ui;
    QDir m_path;
    QList<WidgetPluginStatus *> m_pluginWidgets;
};

#endif //!__WIDGET_PROJECT_SETUP_H__
