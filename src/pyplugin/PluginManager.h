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
#ifndef __PLUGINMANAGER_H__
#define __PLUGINMANAGER_H__

#include <stdafx.h>
#include "PyPlugin.h"
#include "PyThread.h"
#include <QHash>
#include <QList>
#include <functional>
#include <QThread>

using namespace std;

typedef shared_ptr<PyPlugin> PluginRef;
typedef QList<PluginRef> PluginList;
typedef QHash<QString, PluginList> PluginMap;

typedef function<void(PluginRef plugin, int current, int total)> CurrentPluginUpdatedCallback;
typedef function<void(bool success, PluginRef failedPlugin, int failedIndex)> FinishedExecutingPluginCallback;

class PluginHookThreadWorker : public PyThreadWorker
{
    Q_OBJECT
public:
    PluginHookThreadWorker(QString hook, boost::python::dict kwargs, PluginList plugins);
    
signals:
    void nextPluginStarted(PluginRef plugin, int index, int total);
    void finishedExecuting(bool success, PluginRef failedPlugin, int failedIndex);
};

class PluginManager : public QObject
{
    Q_OBJECT
public:
    static PluginManager* Instance();
    static void DestroyInstance();
    static void RegisterPlugin(PluginRef plugin);
    
    PluginManager();
    virtual ~PluginManager();
    
    void registerPlugin(PluginRef plugin);
    PluginList getPluginsForHook(QString hook);
    PluginHookThreadWorker *executePluginsForHook(
        QString hook,
        boost::python::dict args = boost::python::dict()
    );
    void exec(Work w);
    void exec(PyThreadWorker *worker);
    void cleanupPlugins();
    
signals:
    void ExecutingHook(QString hook, PluginRef plugin, int current, int total);
    void ExecutingFinished(bool success);
    
private:
    PyThread m_thread;
    PluginMap m_mapper;
};

#endif // !__PLUGINMANAGER_H__
