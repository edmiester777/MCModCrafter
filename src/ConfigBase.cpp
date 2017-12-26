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
#include "ConfigBase.h"

ConfigBase::ConfigBase()
    : QObject(nullptr)
{
}

ConfigBase::ConfigBase(const ConfigBase& rhs)
{
    if(this != &rhs)
    {
        *this = rhs;
    }
}

ConfigBase::~ConfigBase()
{
}

ConfigBase& ConfigBase::operator=(const ConfigBase& rhs)
{
    m_values = rhs.m_values;
    return *this;
}

bool ConfigBase::Save(QString path)
{
    // opening file for read/write
    QFile file(path);
    if(file.open(QIODevice::ReadWrite | QIODevice::Truncate | QIODevice::Text))
    {
        QJsonObject obj;
        for (JsonValueMemberMap::iterator iter = m_values.begin();
            iter != m_values.end();
            ++iter)
        {
            if(!iter.value().isUndefined())
            {
                obj[iter.key()] = iter.value();
            }
        }
        QJsonDocument doc;
        doc.setObject(obj);
        QByteArray bytes = doc.toJson();
        qint64 written = file.write(bytes);
        if(written < bytes.length())
        {
            L_ERROR("Did not write entire settings file.");
        }
        else
        {
            file.close();
            return true;
        }
    }
    else
    {
        L_ERROR("Could not overwrite config file.");
    }
    file.close();
    return false;
}

bool ConfigBase::Load(QString path)
{
    QFile file(path);
    if(file.exists())
    {
        if(file.open(QIODevice::ReadOnly))
        {
            QByteArray bytes = file.readAll();
            QJsonParseError err;
            QJsonDocument doc = QJsonDocument::fromJson(bytes, &err);
            if(doc.isNull() || doc.isEmpty() || !doc.isObject())
            {
                L_WARN(
                    QString("Failed to parse %1 with message %2")
                    .arg(path, err.errorString())
                );
            }
            else
            {
                QJsonObject obj = doc.object();
                for (JsonValueMemberMap::iterator iter = m_values.begin();
                    iter != m_values.end();
                    ++iter)
                {
                    QString key = iter.key();
                    if(obj.contains(key))
                    {
                        m_values[key] = QJsonValue::fromVariant(obj[key].toVariant());
                    }
                }

                L_INFO(QString("Loaded %1!").arg(path));
                return true;
            }
        }
        else
        {
            L_WARN(QString("Failed to open %1.").arg(path));
        }
    }
    return false;
}

QJsonValue* ConfigBase::AddMember(QString memberName)
{
    m_values[memberName] = QJsonValue();
    return nullptr;
}
