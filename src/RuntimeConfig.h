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

#ifndef __RUNTIMECONFIG_H__
#define __RUNTIMECONFIG_H__

#include "stdafx.h"
#include "ConfigBase.h"

class RuntimeConfig : public ConfigBase
{
public:
    static RuntimeConfig* Instance();

private:
    CONFIG_STRING_PROPERTY(LogsDirectory);
    CONFIG_STRING_PROPERTY(ProjectsDirectory);
    CONFIG_STRING_PROPERTY(DownloadsDirectory);
    CONFIG_STRING_PROPERTY(ForgeDirectory);
    CONFIG_STRING_PROPERTY(ProjectConfigName);
    CONFIG_STRING_PROPERTY(MinecraftForgeUrl);
    CONFIG_STRING_ARRAY_PROPERTY(AvailableHooks);
};

#endif //!__RUNTIMECONFIG_H__
