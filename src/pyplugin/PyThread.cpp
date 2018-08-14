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
#include "PyThread.h"

PyThreadWorker::PyThreadWorker(Work w, QObject *parent) : QObject(parent)
{
    setWork(w);
}

void PyThreadWorker::exec()
{
    try
    {
        L_DEBUG("Beginning work on python thread...");
        if(m_work)
        {
            m_work();
            L_DEBUG("Work on python thread was a success!");
            emit onSuccess();
        }
        else
        {
            L_DEBUG("Execution of work on python thread failed.");
            emit onFailure();
        }
    }
    catch(std::exception &)
    {
        L_DEBUG("Execution of work on python thread failed.");
        emit onFailure();
    }
}

void PyThreadWorker::setWork(Work w)
{
    m_work = w;
}

PyThread::PyThread(QObject* parent) : QThread(parent)
{
    m_stopSignal = false;
}

PyThread::~PyThread()
{
    signalStop();
    m_mtx.lock();
    while(!m_work.empty())
    {
        delete m_work.front();
        m_work.pop();
    }
    m_mtx.unlock();
}

void PyThread::run()
{
    Py_Initialize();
    while(true)
    {
        m_mtx.lock();
        bool stop = m_stopSignal;
        bool empty = m_work.empty();
        m_mtx.unlock();

        if(stop)
            break;

        if(!empty)
        {
            m_mtx.lock();
            PyThreadWorker* work = m_work.front();
            m_work.pop();
            m_mtx.unlock();
            work->exec();
            delete work;
        }
        else
        {
            m_mtx.lock();
            m_workWait.wait(&m_mtx);
            m_mtx.unlock();
        }
    }
    Py_Finalize();
}

void PyThread::add(PyThreadWorker *worker)
{
    L_DEBUG("Adding work to python thread...");
    m_mtx.lock();
    m_work.push(worker);
    m_mtx.unlock();
    m_workWait.wakeAll();
}

void PyThread::signalStop()
{
    L_DEBUG("Python thread signaled to stop.");
    m_mtx.lock();
    m_stopSignal = true;
    m_mtx.unlock();
    m_workWait.wakeAll();
}
