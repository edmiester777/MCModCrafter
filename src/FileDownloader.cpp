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
#include "FileDownloader.h"

FileDownloader::FileDownloader(QObject* parent)
    : QObject(parent),
    m_writeFile(nullptr),
    m_manager(nullptr),
    m_reply(nullptr)
{

}

FileDownloader::~FileDownloader()
{
    if(m_writeFile != nullptr)
    {
        delete m_writeFile;
    }
    if(m_manager != nullptr)
    {
        delete m_manager;
    }
    if(m_reply != nullptr)
    {
        delete m_reply;
    }
}

bool FileDownloader::Download(QUrl url, QString savePath)
{
    if(m_writeFile != nullptr || m_reply != nullptr)
    {
        L_ERROR("Tried to download file while downloading another.");
    }
    
    if(QFile(savePath).exists())
    {
        L_WARN(QString("File exists at %1... Attempting to remove.").arg(savePath));
        if(!QFile(savePath).remove())
        {
            L_ERROR(QString("Failed to remove %1.").arg(savePath));
            return false;
        }
    }

    m_writeFile = new QFile(savePath);
    if(!m_writeFile->open(QIODevice::WriteOnly))
    {
        L_ERROR(QString("Failed to open %1 for write.").arg(savePath));
        delete m_writeFile;
        m_writeFile = nullptr;
        return false;
    }

    m_manager = new QNetworkAccessManager(this);
    m_reply = m_manager->get(QNetworkRequest(url));
    
    connect(m_reply, SIGNAL(readyRead()), this, SLOT(ReadyRead()));
    connect(m_reply, SIGNAL(finished()), this, SLOT(DownloadFinished()));

    return true;
}

void FileDownloader::ReadyRead()
{
    if(!m_writeFile->isOpen())
    {
        L_WARN("Tried to write to file that was closed.");
        return;
    }
    m_writeFile->write(m_reply->readAll());
}

void FileDownloader::DownloadFinished()
{
    // getting if this finished with error.
    bool withError = m_reply->error() != QNetworkReply::NetworkError::NoError;

    // resetting state.
    m_writeFile->close();
    delete m_writeFile;
    delete m_manager;
    m_writeFile = nullptr;
    m_manager = nullptr;
    m_reply = nullptr;

    emit Finished(withError);
}
