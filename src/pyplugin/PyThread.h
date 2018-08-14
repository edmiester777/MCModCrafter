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
#ifndef __PYTHREAD_H__
#define __PYTHREAD_H__

#include <QThread>
#include <QMutex>
#include <QWaitCondition>
#include <functional>
#include <queue>

typedef std::function<void()> Work;
typedef Work SuccessCallback;
typedef Work FailCallback;

class PyThreadWorker : public QObject
{
    Q_OBJECT
public:
    PyThreadWorker(Work w, QObject *parent = Q_NULLPTR);

    void exec();

signals:
    void onSuccess();
    void onFailure();

protected:
    void setWork(Work w);

private:
    Work m_work;
};

class PyThread : public QThread
{
    Q_OBJECT
public:
    PyThread(QObject *parent = Q_NULLPTR);
    virtual ~PyThread();
    void run()override;
    void add(PyThreadWorker* worker);
    void signalStop();
    
private:
    bool m_stopSignal;
    QMutex m_mtx;
    QWaitCondition m_workWait;
    std::queue<PyThreadWorker*> m_work;
};

#endif // !__PYTHREAD_H__
