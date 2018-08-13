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
#include <QTextStream>
#include <QRegExp>

ConfigBase::ConfigBase()
    : QObject(nullptr)
{
}

ConfigBase::ConfigBase(QJsonObject obj)
    : QObject(nullptr)
{
    L_WARN("Usage of ConfigBase::ConfigBase(QJsonObject) is unpredictable."
           " Use ConfigBase::LoadFromObject(QJsonObject) instead."
           " You have been warned!");
    LoadFromObject(obj);
}

ConfigBase::ConfigBase(const ConfigBase& rhs)
    : QObject(nullptr)
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
    m_objects = rhs.m_objects;
    m_arrays = rhs.m_arrays;
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
            if(!iter.value().first.isUndefined())
            {
                obj[iter.key()] = iter.value().first;
            }
        }
        for(JsonObjectMemberMap::iterator iter = m_objects.begin();
            iter != m_objects.end();
            ++iter)
        {
            obj[iter.key()] = iter.value().ToObject();
        }
        for(JsonArrayMemberMap::iterator iter = m_arrays.begin();
            iter != m_arrays.end();
            ++iter)
        {
            obj[iter.key()] = iter.value().first;
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
            // reading file
            QTextStream stream(&file);
            QString text;

            // removing comments
            while(!stream.atEnd())
            {
                QString line = stream.readLine();
                QRegExp exp("\\s{0,}\\/\\/(.*)");
                if(exp.exactMatch(line))
                {
                    line = "";
                }
                else
                {
                    line = line + "\n";
                }
                text += line;
            }

            QJsonParseError err;
            QJsonDocument doc = QJsonDocument::fromJson(text.toUtf8(), &err);
            if(doc.isNull() || doc.isEmpty() || !doc.isObject())
            {
                L_WARN(
                    QString("Failed to parse %1 with message %2")
                    .arg(path, err.errorString())
                );
            }
            else
            {
                LoadFromObject(doc.object());
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

void ConfigBase::LoadFromObject(QJsonObject obj)
{
    for (JsonValueMemberMap::iterator iter = m_values.begin();
        iter != m_values.end();
        ++iter)
    {
        QString key = iter.key();
        if (obj.contains(key))
        {
            L_DEBUG(QString("Found value for property \"%1\".").arg(key));
            m_values[key].first = QJsonValue::fromVariant(obj[key].toVariant());
        }
        else
        {
            L_DEBUG(QString("Key \"%1\" not found.").arg(key));
            m_values[key].first.fromVariant(QVariant());
        }
    }
    for (JsonObjectMemberMap::iterator iter = m_objects.begin();
        iter != m_objects.end();
        ++iter)
    {
        QString key = iter.key();
        if (obj.contains(key) && obj[key].isObject())
        {
            L_DEBUG(QString("Found object for property \"%1\".").arg(key));
            iter.value().LoadFromObject(obj[key].toObject());
        }
        else
        {
            L_DEBUG(QString("Key \"%1\" not found or was an unexpected type.").arg(key));
            iter.value().LoadFromObject(QJsonObject());
        }
    }
    for (JsonArrayMemberMap::iterator iter = m_arrays.begin();
        iter != m_arrays.end();
        ++iter)
    {
        QString key = iter.key();
        if (obj.contains(key) && obj[key].isArray())
        {
            L_DEBUG(QString("Found array for property \"%1\".").arg(key));
            m_arrays[key].first = obj[key].toArray();
        }
        else
        {
            L_DEBUG(QString("Key \"%1\" not found or was an unexpected type.").arg(key));
        }
    }
}


QJsonObject ConfigBase::ToObject()
{
    QJsonObject obj;

    for(JsonValueMemberMap::iterator iter = m_values.begin();
        iter != m_values.end();
        ++iter)
    {
        obj.insert(iter.key(), m_values[iter.key()].first);
    }
    for(JsonObjectMemberMap::iterator iter = m_objects.begin();
        iter != m_objects.end();
        ++iter)
    {
        obj.insert(iter.key(), m_objects[iter.key()].ToObject());
    }
    for(JsonArrayMemberMap::iterator iter = m_arrays.begin();
        iter != m_arrays.end();
        ++iter)
    {
        obj.insert(iter.key(), m_arrays[iter.key()].first);
    }

    return obj;
}

boost::python::dict ConfigBase::ToPythonObject()
{
    using namespace boost::python;
    dict d;
    for(JsonValueMemberMap::iterator iter = m_values.begin();
        iter != m_values.end();
        ++iter)
    {
        d[iter.key().toStdString()] = iter.value().second();
    }
    for(JsonObjectMemberMap::iterator iter = m_objects.begin();
        iter != m_objects.end();
        ++iter)
    {
        d[iter.key().toStdString()] = m_objects[iter.key()].ToPythonObject();
    }
    for(JsonArrayMemberMap::iterator iter = m_arrays.begin();
        iter != m_arrays.end();
        ++iter)
    {
        d[iter.key().toStdString()] = m_arrays[iter.key()].second();
    }

    return d;
}

QJsonValue* ConfigBase::AddMember(QString memberName, MemberPythonGetter pyget)
{
    m_values[memberName] = Member(QJsonValue(), pyget);
    return nullptr;
}

QJsonArray * ConfigBase::AddArrayMember(QString memberName, ArrayPythonGetter pyget)
{
    m_arrays[memberName] = Array(QJsonArray(), pyget);
    return nullptr;
}