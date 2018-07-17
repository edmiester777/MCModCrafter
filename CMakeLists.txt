# MCModCrafter Program to help introduce people to minecraft modding and
# to speed development time via logic visualization.
#
# Copyright (C) 2017 Edward Callahan
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <https://www.gnu.org/licenses/>.

cmake_minimum_required(VERSION 2.8.11)

project(MCModCrafter)

set(CMAKE_AUTOMOC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# Setting qt install path
if(DEFINED QTDIR)
    list(APPEND CMAKE_PREFIX_PATH "${QTDIR}")
elseif(DEFINED ENV{QTDIR})
    list(APPEND CMAKE_PREFIX_PATH "$ENV{QTDIR}")
endif()

# Setting boost install path
set(Boost_USE_STATIC_LIBS ON)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME OFF)
if(DEFINED BOOSTDIR)
    set(BOOSTROOT "${BOOSTDIR}")
elseif(DEFINED ENV{BOOSTDIR})
    set(BOOSTROOT "$ENV{BOOSTDIR}")
endif()

# Finding required qt packages
find_package(Qt5 COMPONENTS
    Core
    Widgets
    Network
    REQUIRED
)

# finding other packages.
FIND_PACKAGE(PythonInterp)

if (PYTHONINTERP_FOUND)
  if (UNIX AND NOT APPLE)
    if (PYTHON_VERSION_MAJOR EQUAL 3)
        FIND_PACKAGE(Boost COMPONENTS python${PYTHON_VERSION_SUFFIX})
        FIND_PACKAGE(PythonInterp 3)
        FIND_PACKAGE(PythonLibs 3 REQUIRED)
    else()
        FIND_PACKAGE(Boost COMPONENTS python)
        FIND_PACKAGE(PythonInterp)
        FIND_PACKAGE(PythonLibs REQUIRED)
    endif()
  else()
    if (PYTHON_VERSION_MAJOR EQUAL 3)
        FIND_PACKAGE(Boost COMPONENTS python${PYTHON_VERSION_MAJOR}${PYTHON_VERSION_MINOR})
        FIND_PACKAGE(PythonInterp 3)
        FIND_PACKAGE(PythonLibs 3 REQUIRED)
    else()
        FIND_PACKAGE(Boost COMPONENTS python${PYTHON_VERSION_MAJOR}${PYTHON_VERSION_MINOR})
        FIND_PACKAGE(PythonInterp)
        FIND_PACKAGE(PythonLibs REQUIRED)
    endif()
  endif()
else()
    message("Python not found")
endif()

if(MSVC)
    add_definitions(-DBOOST_PYTHON_STATIC_LIB)
endif()

message(STATUS "PYTHON_LIBRARIES = ${PYTHON_LIBRARIES}")
message(STATUS "PYTHON_EXECUTABLE = ${PYTHON_EXECUTABLE}")
message(STATUS "PYTHON_INCLUDE_DIRS = ${PYTHON_INCLUDE_DIRS}")
message(STATUS "Boost_LIBRARIES = ${Boost_LIBRARIES}")

# Source definitions
set(MCMODCRAFTER_HEADERS
    src/stdafx.h
    src/ConfigBase.h
    src/RuntimeConfig.h
    src/ProjectConfig.h
    src/FileDownloader.h
    src/ZipUtil.h
    src/WindowMCModCrafter.h
    src/WidgetSelectProject.h
    src/DialogCreateProject.h

    # third party
    third_party/SimpleQtLogger/simpleQtLogger.h
)

set(MCMODCRAFTER_SOURCES
    src/main.cpp
    src/stdafx.cpp
    src/ConfigBase.cpp
    src/RuntimeConfig.cpp
    src/FileDownloader.cpp
    src/ZipUtil.cpp
    src/WindowMCModCrafter.cpp
    src/WidgetSelectProject.cpp
    src/DialogCreateProject.cpp

    # third party
    third_party/SimpleQtLogger/simpleQtLogger.cpp
)

set(MCMODCRAFTER_FORMS
    src/forms/WindowMCModCrafter.ui
    src/forms/WidgetSelectProject.ui
    src/forms/DialogCreateProject.ui
)

set(MCMODCRAFTER_RESOURCES
    src/res/MCModCrafter.qrc
)

qt5_wrap_ui(MCMODCRAFTER_UI_HEADERS ${MCMODCRAFTER_FORMS})
qt5_add_resources(MCMODCRAFTER_QRC_SOURCES ${MCMODCRAFTER_RESOURCES})

add_executable(frontend
    ${MCMODCRAFTER_HEADERS}
    ${MCMODCRAFTER_SOURCES}
    ${MCMODCRAFTER_UI_HEADERS}
    ${MCMODCRAFTER_QRC_SOURCES}
)

target_link_libraries(frontend
    Qt5::Core
    Qt5::Widgets
    Qt5::Network
    ${Boost_LIBRARIES}
    ${PYTHON_LIBRARIES}
)
message(STATUS "${Boost_LIBRARIES}")

include_directories(
    third_party/SimpleQtLogger
    ${Boost_INCLUDE_DIRS}
    ${PYTHON_INCLUDE_DIRS}
)
target_include_directories(frontend PUBLIC "${CMAKE_SOURCE_DIR}/src")

# Copying python libraries on build.
get_filename_component(PYTHON_LIBS_DIR ${PYTHON_EXECUTABLE} DIRECTORY)
file(GLOB_RECURSE pylibfiles RELATIVE "${PYTHON_LIBS_DIR}" "*")
if(MSVC)
    # Preparing for cross platform build.
    # Copying python libraries.
    add_custom_command(
        TARGET frontend POST_BUILD
        COMMAND xcopy ARGS "\"${PYTHON_LIBS_DIR}/Lib\"" "\"$<TARGET_FILE_DIR:frontend>/pylibs\""
            /E /D /Y /H /R /C /I
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    )
else()
endif()


# Enabling install during build.
if(WIN32)
    get_target_property(QT5_QMAKE_EXECUTABLE Qt5::qmake IMPORTED_LOCATION)
    get_filename_component(QT5_WINDEPLOYQT_EXECUTABLE ${QT5_QMAKE_EXECUTABLE} PATH)
    set(QT5_WINDEPLOYQT_EXECUTABLE "${QT5_WINDEPLOYQT_EXECUTABLE}/windeployqt.exe")

    add_custom_command(TARGET frontend POST_BUILD
    COMMAND ${QT5_WINDEPLOYQT_EXECUTABLE} --qmldir ${CMAKE_SOURCE_DIR} $<TARGET_FILE_DIR:frontend>)
endif(WIN32)

# Setting MSVC flags
if(MSVC)
    set(CMAKE_CXX_FLAGS "/D_WIN32_WINNT=0x0600 /MP /EHsc")
endif()
