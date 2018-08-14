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

#include "WidgetPluginStatus.h"
#include <QDir>
#include <QMessageBox>

WidgetPluginStatus::WidgetPluginStatus(PluginRef plugin, QWidget *parent)
{
    m_plugin = plugin;
    m_ui.setupUi(this);
    setInitialState();
}

void WidgetPluginStatus::setRunning()
{
    // setting up animated label
    QMovie *movie = new QMovie(":/MCModCrafter/img/loading-spinner.gif");
    m_ui.imageLabel->setMovie(movie);
    movie->start();
    
    m_ui.statusLabel->setVisible(true);
    m_ui.imageLabel->setVisible(true);
}

void WidgetPluginStatus::setSuccess()
{
    m_ui.imageLabel->setVisible(true);
    m_ui.statusLabel->setVisible(true);
    if(m_ui.imageLabel->movie())
        delete m_ui.imageLabel->movie();
    QMovie *movie = new QMovie(":/MCModCrafter/img/checkmark.png");
    m_ui.imageLabel->setMovie(movie);
    movie->start();
}

void WidgetPluginStatus::setError()
{
    m_ui.imageLabel->setVisible(true);
    m_ui.statusLabel->setVisible(true);
    m_ui.statusLabel->setText("An error occurred while running this plugin.");
    if(m_ui.imageLabel->movie())
        delete m_ui.imageLabel->movie();
    QMovie *movie = new QMovie(":/MCModCrafter/img/error.png");
    m_ui.imageLabel->setMovie(movie);
    movie->start();
}

void WidgetPluginStatus::setInitialState()
{
    m_ui.titleLabel->setText(QString::fromStdString(m_plugin->getName()));
    m_ui.statusLabel->setText(QString::fromStdString(m_plugin->getStatusText()));
    m_ui.statusLabel->setVisible(false);
    m_ui.imageLabel->setVisible(false);
}

