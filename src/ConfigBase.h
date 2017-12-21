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

#ifndef __CONFIGBASE_H__
#define __CONFIGBASE_H__

#include "stdafx.h"

class ConfigBase;

/**
 * Definition for a list of json values for shorthand use later.
 */
typedef QMap<QString, QJsonValue*> JsonValueMemberMap;

typedef void(*JsonSetter)(void*, QJsonValue*);

/**
 * Definition for a list of setters for properties.
 */
typedef QMap<QString, void(*)(void*, QJsonValue*)> JsonValueSetterMap;

/**
 * Macro to define a config property in the config standard.
 *
 * @param type Type of object being declared.
 * @param name Name (non-string) for object.
 * @param getterFunc Function used on QVariant used to get correct value.
 */
#define CONFIG_PROPERTY(cls, type, name, getterFunc) \
    friend void Set##name##Raw(void*, QJsonValue*); \
    private: QJsonValue* m_##name = AddMember(#name, &Set##name##Raw); \
    private: static void Set##name##Raw(void* p, QJsonValue* value) \
    { \
        cls* obj = static_cast<cls*>(p); \
        if(obj->m_##name != nullptr) \
        { \
            delete obj->m_##name; \
        } \
        obj->m_values[#name] = value; \
        obj->m_##name = value;\
    } \
    public: void Set##name(type value) { m_##name->fromVariant(value); } \
    public: type Get##name() { return m_##name->##getterFunc##(); }

#define CONFIG_STRING_PROPERTY(cls, name) CONFIG_PROPERTY(##cls, QString, ##name, toString)
#define CONFIG_BOOL_PROPERTY(cls, name) CONFIG_PROPERTY(##cls, bool, ##name, toBool)
#define CONFIG_INT_PROPERTY(cls, name) CONFIG_PROPERTY(##cls, int, ##name, toInt)
#define CONFIG_DOUBLE_PROPERTY(cls, name) CONFIG_PROPERTY(##cls, double, ##name, toDouble)

/**
 * @brief Config base class.
 *
 * Here we have all the core definitions for configurations that can be
 * easily extended later for different types of configurations such as
 * projects and global settings.
 */
class ConfigBase : public QObject
{
Q_OBJECT
public:
    /**
     * @brief Constructor
     *
     * Construct a configuration.
     *
     * @param path Path to file to save/load to/from.
     */
    ConfigBase(QString path);
    virtual ~ConfigBase();

    /**
     *
     */
    bool Save();
    bool Load();

protected:
    QJsonValue* AddMember(QString memberName, JsonSetter setter);
    JsonValueMemberMap m_values;
    JsonValueSetterMap m_setters;

private:
    QString m_path;
};

#endif //!__CONFIGBASE_H__