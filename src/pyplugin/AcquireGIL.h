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
#ifndef __ACQUIRE_GIL_H__
#define __ACQUIRE_GIL_H__

#include <stdafx.h>

/**
 * @Brief support for multi-threading.
 *
 * Because of the Global Interpreter Lock (GIL) in python interpreter,
 * it makes it challenging to support calls from different threads.
 * This class, when declared, will store the GIL so we may interact
 * with python from other threads.
 *
 * This should be used infrequently and only as-needed. Should delete
 * right when finished using.
 */
class AcquireGIL 
{
public:
    AcquireGIL();
    ~AcquireGIL();

private:
    PyGILState_STATE m_state;
};

#endif // !__ACQUIRE_GIL_H__
