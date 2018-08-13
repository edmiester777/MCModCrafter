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
#include <functional>

class ConfigBase;

/**
 * Definition of a funciton pointing to a members generated python getter
 */
typedef std::function<boost::python::object()> MemberPythonGetter;

/**
 * Definition of a function pointing to an array members generated
 * python getter.
 */
typedef std::function<boost::python::list()> ArrayPythonGetter;

/**
 * Definition of a member pair.
 */
typedef QPair<QJsonValue, MemberPythonGetter> Member;

/**
 * Definition of an array member pair.
 */
typedef QPair<QJsonArray, ArrayPythonGetter> Array;

/**
 * Definition for a list of json values for shorthand use later.
 */
typedef QMap<QString, Member> JsonValueMemberMap;

/**
 * Definition for object members.
 */
typedef QMap<QString, ConfigBase> JsonObjectMemberMap;

/**
 * Definition for an array of json values.
 */
typedef QMap<QString, Array> JsonArrayMemberMap;

/**
 * Macro to define a config property in the config standard.
 *
 * @param type Type of object being declared.
 * @param name Name (non-string) for object.
 * @param getterFunc Function used on QVariant used to get correct value.
 * @param toPythonFunc Function that will be used to convert the QVariant to a boost::python::object
 */
#define CONFIG_PROPERTY(type, name, getterFunc, toPythonFunc) \
    public: void Set##name(type value) { m_values[#name].first = QJsonValue::fromVariant(value); } \
    public: type Get##name()const { return m_values[#name].first.getterFunc(); } \
    private: boost::python::object GetPython##name()const { return boost::python::object(m_values[#name].first.toPythonFunc()); } \
    public: QJsonValue* ___m_donotaccess_##name = AddMember( \
        #name, \
        [this]() -> boost::python::object { \
            return this->GetPython##name(); \
        } \
    );

#define CONFIG_STRING_PROPERTY(name) CONFIG_PROPERTY(QString, name, toString, toString().toStdString)
#define CONFIG_BOOL_PROPERTY(name) CONFIG_PROPERTY(bool, name, toBool, toBool)
#define CONFIG_INT_PROPERTY(name) CONFIG_PROPERTY(int, name, toInt, toInt)
#define CONFIG_DOUBLE_PROPERTY(name) CONFIG_PROPERTY(double, name, toDouble, toDouble)

/**
 * Use this when defining a member that is an object. The member will have
 * to be a child of ConfigBase.
 *
 * @param type Type of object that this member will be (child of ConfigBase).
 * @param name Name of member.
 */
#define CONFIG_OBJECT_PROPERTY(type, name) \
    private: ConfigBase* ___m_donotaccess_##name = AddObjectMember<type>(#name); \
    public: type* ##name() { return static_cast<type*>(&m_objects[#name]); }

/**
 * Use this when defining an array of primitive types supported by JSON.
 *
 * @param type JSON primitive type that will be stored in this array.
 * @param name Name of member (without plurality) that will be saved/loaded.
 * @param getterFunc Function to call on QJsonValue when getting stored value.
 * @param toPythonFunc Function to convert the QVariant to a boost::python::object
 */
#define CONFIG_ARRAY_PROPERTY(type, name, getterFunc, toPythonFunc) \
    public: void Add##name(type value) { m_arrays[#name].first.append(value); } \
    public: QVector<type> name()const \
    { \
        QVector<type> vec; \
        QJsonArray arr = m_arrays[#name].first; \
        for(QJsonArray::iterator iter = arr.begin(); \
            iter != arr.end(); \
            ++iter) \
        { \
            vec.push_back((*iter).getterFunc()); \
        } \
        return vec; \
    } \
    public: boost::python::list GetPython##name()const \
    { \
        boost::python::list l; \
        QJsonArray arr = m_arrays[#name].first; \
        for(QJsonArray::iterator iter = arr.begin(); \
            iter != arr.end(); \
            ++iter) \
        { \
            l.append((*iter).toPythonFunc()); \
        } \
        return l; \
    } \
    private: QJsonArray* ___m_donotaccess_##name = AddArrayMember( \
        #name, \
        [this]() -> boost::python::list { \
            return GetPython##name(); \
        } \
    );


#define CONFIG_STRING_ARRAY_PROPERTY(name) CONFIG_ARRAY_PROPERTY(QString, name, toString, toString().toStdString)
#define CONFIG_BOOL_ARRAY_PROPERTY(name) CONFIG_ARRAY_PROPERTY(bool, name, toBool, toBool)
#define CONFIG_INT_ARRAY_PROPERTY(name) CONFIG_ARRAY_PROPERTY(int, name, toInt, toInt)
#define CONFIG_DOUBLE_ARRAY_PROPERTY(name) CONFIG_ARRAY_PROPERTY(double, name, toDouble, toDouble)

/**
 * Use this when defining an array of object types. These objects must all be of the same
 * type, and be a child of ConfigBase.
 *
 * @param class Class (child of ConfigBase) that you expect to be stored in the array.
 * @param name Name (without plurality) for the member.
 */
#define CONFIG_OBJECT_ARRAY_PROPERTY(cls, name) \
    public: void Add##name(cls value) { m_arrays[#name].append(value.ToObject()); } \
    public: QVector<cls> ##name##s()const \
    { \
        QVector<cls> vec; \
        QJsonArray arr = m_arrays[#name]; \
        for(QJsonArray::iterator iter = arr.begin(); \
            iter != arr.end(); \
            ++iter) \
        { \
            cls tmp = cls(); \
            tmp.LoadFromObject((*iter).toObject()); \
            vec.push_back(tmp); \
        } \
        return vec; \
    } \
    public: boost::python::list GetPython##name()const \
    { \
        boost::python::list l; \
        QVector<cls> objs = ##name##s(); \
        for(int i = 0; i < objs.length(); ++i) \
            l.append(objs[i].ToPythonObject()); \
        return l; \
    } \
    private: QJsonArray* ___m_donotaccess_##name = AddArrayMember( \
        #name, \
        [this]() -> boost::python::list { \
            return GetPython##name(); \
        } \
    );

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
     * Load this configuration from a json object.
     *
     * @param obj Object to load from.
     */
    void LoadFromObject(QJsonObject obj);

    /**
     * @brief Convert this config to an object.
     *
     * This will do the opposite of loading.
     *
     * @return Config in JSON form.
     */
    QJsonObject ToObject();

    /**
     * @brief Convert this config to a python object.
     *
     * This will do the same thing as ToObject() however the resulting object
     * will be a python compatable one.
     *
     * @return Config in python form.
     */
    boost::python::dict ToPythonObject();

protected:
    QJsonValue* AddMember(QString memberName, MemberPythonGetter pyget);
    template<class T>
    ConfigBase* AddObjectMember(QString memberName);
    QJsonArray* AddArrayMember(QString memberName, ArrayPythonGetter pyget);
    JsonValueMemberMap m_values;
    JsonObjectMemberMap m_objects;
    JsonArrayMemberMap m_arrays;
};

template<class T>
ConfigBase* ConfigBase::AddObjectMember(QString memberName)
{
    T member;
    m_objects[memberName] = member;
    return nullptr;
}

#endif //!__CONFIGBASE_H__
