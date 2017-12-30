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

#ifndef __FILE_DOWNLOADER_H__
#define __FILE_DOWNLOADER_H__

#include "stdafx.h"
#include <QtNetwork>

/**
 * @brief File downloader for use with HTTP.
 *
 * This HTTP file downloader is a simple implementation with
 * very limited functionality for use with MCModCrafters minimal
 * setup implementation.
 */
class FileDownloader : public QObject
{
    Q_OBJECT
public:
    FileDownloader(QObject* parent = nullptr);
    virtual ~FileDownloader();

    /**
     * @brief Begin file download.
     *
     * Begin the download from an HTTP url.
     *
     * @param url URL to download from.
     * @param savePath Path to save file to.
     * @return Successfully started download.
     */
    bool Download(QUrl url, QString savePath);

private slots:
    /**
     * Network reply read callback.
     */
    void ReadyRead();

    /**
     * Network reply finished callback.
     */
    void DownloadFinished();

signals:
    /**
     * Signal for when a download finishes.
     *
     * @param withError Tells you if there was an error
     * during download.
     */
    void Finished(bool withError);

private:
    QFile* m_writeFile;
    QNetworkAccessManager* m_manager;
    QNetworkReply* m_reply;
};

#endif //!__FILE_DOWNLOADER_H__