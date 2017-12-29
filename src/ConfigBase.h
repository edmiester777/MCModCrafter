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
typedef QMap<QString, QJsonValue> JsonValueMemberMap;

/**
 * Definition for object members.
 */
typedef QMap<QString, ConfigBase> JsonObjectMemberMap;

/**
 * Macro to define a config property in the config standard.
 *
 * @param cls Class that you are adding this property to.
 * @param type Type of object being declared.
 * @param name Name (non-string) for object.
 * @param getterFunc Function used on QVariant used to get correct value.
 */
#define CONFIG_PROPERTY(type, name, getterFunc) \
    private: QJsonValue* ___m_donotaccess_##name = AddMember(#name); \
    public: void Set##name(type value) { m_values[#name] = QJsonValue::fromVariant(value); } \
    public: type Get##name() { return m_values[#name].##getterFunc##(); }

#define CONFIG_STRING_PROPERTY(name) CONFIG_PROPERTY(QString, ##name, toString)
#define CONFIG_BOOL_PROPERTY(name) CONFIG_PROPERTY(bool, ##name, toBool)
#define CONFIG_INT_PROPERTY(name) CONFIG_PROPERTY(int, ##name, toInt)
#define CONFIG_DOUBLE_PROPERTY(name) CONFIG_PROPERTY(double, ##name, toDouble)

#define CONFIG_OBJECT_PROPERTY(type, name) \
    private: ConfigBase* ___m_donotaccess_##name = AddObjectMember<type>(#name); \
    public: type* ##name() { return static_cast<type*>(&m_objects[#name]); }

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
     */
    ConfigBase();
    ConfigBase(QJsonObject obj);
    ConfigBase(const ConfigBase& rhs);
    virtual ~ConfigBase();
    virtual ConfigBase& operator=(const ConfigBase& rhs);

    /**
     * @brief Save this config.
     *
     * Save this config to a said path.
     *
     * @param path Path to file to save to.
     * @return Success
     */
    bool Save(QString path);

    /**
     * @brief Load a config from disk.
     *
     * Load a saved configuration from a file.
     *
     * @param path Path to load from.
     * @return Success
     */
    bool Load(QString path);

    /**
     * @brief Convert this config to an object.
     *
     * This will do the opposite of loading.
     *
     * @return Config in JSON form.
     */
    QJsonObject ToObject();

protected:
    QJsonValue* AddMember(QString memberName);
    template<class T>
    ConfigBase* AddObjectMember(QString memberName);
    JsonValueMemberMap m_values;
    JsonObjectMemberMap m_objects;

private:
    /**
     * Load this configuration from a json object.
     *
     * @param obj Object to load from.
     */
    void LoadFromObject(QJsonObject obj);
};

template<class T>
ConfigBase* ConfigBase::AddObjectMember(QString memberName)
{
    T member;
    m_objects[memberName] = member;
    return nullptr;
}

#endif //!__CONFIGBASE_H__