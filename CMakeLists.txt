#/****************************************************************************
# Copyright (c) 2013-2014 cocos2d-x.org
# Copyright (c) 2015 Chukong Technologies Inc.
#
# http://www.cocos2d-x.org
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:

# The above copyright notice and this permission notice shall be included in
# all copies or substantial portions of the Software.

# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
# THE SOFTWARE.
# ****************************************************************************/
cmake_policy(SET CMP0017 NEW)

cmake_minimum_required(VERSION 3.1)

set(APP_NAME Butcher)
project (${APP_NAME})

set(COCOS2D_ROOT ${CMAKE_SOURCE_DIR}/cocos2d)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${COCOS2D_ROOT}/cmake/Modules/")
include(CocosBuildHelpers)

# libcocos2d
set(BUILD_CPP_EMPTY_TEST OFF CACHE BOOL "turn off build cpp-empty-test")
set(BUILD_CPP_TESTS OFF CACHE BOOL "turn off build cpp-tests")
set(BUILD_LUA_LIBS OFF CACHE BOOL "turn off build lua related targets")
set(BUILD_JS_LIBS OFF CACHE BOOL "turn off build js related targets")
add_subdirectory(${COCOS2D_ROOT})

# Some macro definitions
if(WINDOWS)
  if(BUILD_SHARED_LIBS)
    ADD_DEFINITIONS (-D_USRDLL -D_EXPORT_DLL_ -D_USEGUIDLL -D_USREXDLL -D_USRSTUDIODLL)
  else()
    ADD_DEFINITIONS (-DCC_STATIC)
  endif()

  ADD_DEFINITIONS (-DCOCOS2DXWIN32_EXPORTS -D_WINDOWS -DWIN32 -D_WIN32)
  set(PLATFORM_FOLDER win32)
elseif(MACOSX OR APPLE)
  ADD_DEFINITIONS (-DCC_TARGET_OS_MAC)
  ADD_DEFINITIONS (-DUSE_FILE32API)
  set(PLATFORM_FOLDER mac)
elseif(LINUX)
  ADD_DEFINITIONS(-DLINUX)
  set(PLATFORM_FOLDER linux)
elseif(ANDROID)
  ADD_DEFINITIONS (-DUSE_FILE32API)
  set(PLATFORM_FOLDER android)
else()
  message( FATAL_ERROR "Unsupported platform, CMake will exit" )
endif()


# Compiler options
if(MSVC)
  ADD_DEFINITIONS(-D_CRT_SECURE_NO_WARNINGS -D_SCL_SECURE_NO_WARNINGS
                  -wd4251 -wd4244 -wd4334 -wd4005 -wd4820 -wd4710
                  -wd4514 -wd4056 -wd4996 -wd4099)
else()
  set(CMAKE_C_FLAGS_DEBUG "-g -Wall -DCOCOS2D_DEBUG=1")
  set(CMAKE_CXX_FLAGS_DEBUG ${CMAKE_C_FLAGS_DEBUG})
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c99")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wno-deprecated-declarations -Wno-reorder")
  if(CLANG)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++")
  endif()
endif(MSVC)


set(PLATFORM_SPECIFIC_SRC)
set(PLATFORM_SPECIFIC_HEADERS)

if(MACOSX OR APPLE)
    set(PLATFORM_SPECIFIC_SRC
      proj.ios_mac/mac/main.cpp
    )
elseif(LINUX)
    set(PLATFORM_SPECIFIC_SRC
      proj.linux/main.cpp
    )
elseif ( WIN32 )
    set(PLATFORM_SPECIFIC_SRC
      proj.win32/main.cpp
    )
    set(PLATFORM_SPECIFIC_HEADERS
      proj.win32/main.h
      proj.win32/resource.h
    )
elseif(ANDROID)
    set(PLATFORM_SPECIFIC_SRC
      proj.android-studio/app/jni/hellocpp/main.cpp
    )
endif()

include_directories(
  /usr/local/include/GLFW
  /usr/include/GLFW
  ${COCOS2D_ROOT}/cocos
  ${COCOS2D_ROOT}/cocos/platform
  ${COCOS2D_ROOT}/cocos/audio/include/
  Classes
)
if ( WIN32 )
  include_directories(
  ${COCOS2D_ROOT}/external/glfw3/include/win32
  ${COCOS2D_ROOT}/external/win32-specific/gles/include/OGLES
)
endif( WIN32 )

set(GAME_SRC
  Classes/AppDelegate.cpp
  Classes/butcher.cpp
  Classes/hud_layer.cpp
  Classes/actors/actor.cpp
  Classes/actors/object.cpp
  Classes/actors/character.cpp
  Classes/actors/player.cpp
  Classes/actors/monster.cpp
  Classes/data/actors_database.cpp
  Classes/data/levels_database.cpp
  Classes/dungeon/dungeon_layer.cpp
  Classes/dungeon/dungeon_state.cpp
  Classes/actors/actions/move_action.cpp
  Classes/actors/actions/attack_action.cpp
  Classes/actors/actions/die_action.cpp
  Classes/utils/utils.cpp
  Classes/utils/target.cpp
  Classes/actors/ai/ai.cpp
  Classes/actors/ai/fsm/fsm.cpp
  Classes/actors/ai/fsm/states/fsm_state.cpp
  Classes/actors/ai/fsm/states/idle_state.cpp
  Classes/actors/ai/fsm/states/wandering_state.cpp
  Classes/actors/ai/fsm/states/move_to_target_state.cpp
  Classes/actors/ai/fsm/states/melee_attack_state.cpp
  Classes/lib/permissive-fov.cc
  Classes/dungeon/generators/cave_grid_generator.cpp
  Classes/dungeon/generators/tmx_builder.cpp
  Classes/lib/DungeonMaker.cpp
  Classes/dungeon/generators/dungeon_maker_generator.cpp
  Classes/utils/profiler.cpp
  Classes/loading_scene.cpp
  Classes/actors/instances/stairs_down.cpp
  Classes/actors/instances/stairs_up.cpp
  Classes/dungeon/level_manager.cpp
  ${PLATFORM_SPECIFIC_SRC}
)

set(GAME_HEADERS
  Classes/AppDelegate.h
  Classes/butcher.h
  Classes/hud_layer.h
  Classes/actors/actor.h
  Classes/actors/character.h
  Classes/actors/player.h
  Classes/actors/monster.h
  Classes/actors/attribute_type.h
  Classes/data/actors_database.h
  Classes/data/levels_database.h
  Classes/dungeon/dungeon_layer.h
  Classes/dungeon/dungeon_state.h
  Classes/actors/actions/actor_action.h
  Classes/actors/actions/move_action.h
  Classes/actors/actions/attack_action.h
  Classes/actors/actions/die_action.h
  Classes/utils/directions.h
  Classes/utils/utils.h
  Classes/utils/target.h
  Classes/actors/ai/ai.h
  Classes/actors/ai/fsm/fsm.h
  Classes/actors/ai/fsm/fsm_state_type.h
  Classes/actors/ai/fsm/states/fsm_state.h
  Classes/actors/ai/fsm/states/idle_state.h
  Classes/actors/ai/fsm/states/wandering_state.h
  Classes/actors/ai/fsm/states/move_to_target_state.h
  Classes/actors/ai/fsm/states/melee_attack_state.h
  Classes/lib/permissive-fov.h
  Classes/lib/permissive-fov-cpp.h
  Classes/dungeon/tile_gid.h
  Classes/utils/grid.h
  Classes/utils/profiler.h
  Classes/loading_scene.h
  Classes/dungeon/generators/grid_generator.h
  Classes/dungeon/generators/cave_grid_generator.h
  Classes/lib/DungeonMaker.h
  Classes/dungeon/generators/tmx_builder.h
  Classes/dungeon/generators/dungeon_maker_generator.h
  Classes/actors/object.h
  Classes/actors/instances/stairs_down.h
  Classes/actors/instances/stairs_up.h
  Classes/dungeon/level_manager.h
  Classes/levels_generated.h
  ${PLATFORM_SPECIFIC_HEADERS}
)

if( ANDROID )
    add_library(${APP_NAME} SHARED ${GAME_SRC} ${GAME_HEADERS})
    IF(CMAKE_BUILD_TYPE MATCHES RELEASE)
        ADD_CUSTOM_COMMAND(TARGET ${APP_NAME} POST_BUILD COMMAND ${CMAKE_STRIP} lib${APP_NAME}.so)
    ENDIF()
else()
    add_executable(${APP_NAME} ${GAME_SRC} ${GAME_HEADERS})
endif()

target_link_libraries(${APP_NAME} cocos2d)

set(APP_BIN_DIR "${CMAKE_BINARY_DIR}/bin")

set_target_properties(${APP_NAME} PROPERTIES
     RUNTIME_OUTPUT_DIRECTORY  "${APP_BIN_DIR}")

if ( WIN32 )
  #also copying dlls to binary directory for the executable to run
  pre_build(${APP_NAME}
    COMMAND ${CMAKE_COMMAND} -E remove_directory ${APP_BIN_DIR}/Resources
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/Resources ${APP_BIN_DIR}/Resources
    COMMAND ${CMAKE_COMMAND} -E copy ${COCOS2D_ROOT}/external/win32-specific/gles/prebuilt/glew32.dll ${APP_BIN_DIR}/${CMAKE_BUILD_TYPE}
	COMMAND ${CMAKE_COMMAND} -E copy ${COCOS2D_ROOT}/external/win32-specific/zlib/prebuilt/zlib1.dll ${APP_BIN_DIR}/${CMAKE_BUILD_TYPE}
	)
elseif( ANDROID )

else()
  pre_build(${APP_NAME}
    COMMAND ${CMAKE_COMMAND} -E remove_directory ${APP_BIN_DIR}/Resources
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/Resources ${APP_BIN_DIR}/Resources
    )

endif()
