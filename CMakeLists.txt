cmake_minimum_required(VERSION 3.16...3.26)

include("${CMAKE_CURRENT_SOURCE_DIR}/cmake/common/bootstrap.cmake" NO_POLICY_SCOPE)

project(${_name} VERSION ${_version})

option(ENABLE_FRONTEND_API "Use obs-frontend-api for UI functionality" OFF)
option(ENABLE_QT "Use Qt functionality" OFF)

include(avermedia_deps.cmake) # setup for AVerMedia parameters and dependencies

include(compilerconfig)
include(defaults)
include(helpers)

add_library(${CMAKE_PROJECT_NAME} MODULE)

find_package(libobs REQUIRED)
target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE OBS::libobs)

if(ENABLE_FRONTEND_API)
  find_package(obs-frontend-api REQUIRED)
  target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE OBS::obs-frontend-api)
endif()

if(ENABLE_QT)
  find_qt(COMPONENTS Widgets Core Gui Network)
  target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE Qt::Core Qt::Widgets Qt::Gui Qt::Network)
  target_compile_options(
    ${CMAKE_PROJECT_NAME} PRIVATE $<$<C_COMPILER_ID:Clang,AppleClang>:-Wno-quoted-include-in-framework-header
                                  -Wno-comma>)
  set_target_properties(
    ${CMAKE_PROJECT_NAME}
    PROPERTIES AUTOMOC ON
               AUTOUIC ON
               AUTORCC ON)
endif()

target_sources(${CMAKE_PROJECT_NAME} PRIVATE src/plugin-main.cpp)

set_target_properties_plugin(${CMAKE_PROJECT_NAME} PROPERTIES OUTPUT_NAME ${_name})

# AVerMedia Module
target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE
	${CMAKE_CURRENT_SOURCE_DIR}/sdk/plugin-module/include
)
if(WIN32)
    target_add_resource(${CMAKE_PROJECT_NAME} sdk/ap_32.ico) # OPFSC-638
    target_sources(${CMAKE_PROJECT_NAME} PRIVATE
        src/ObsCloseHelper.h
        src/ObsCloseHelper.cpp
    )
    target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/sdk/plugin-module/lib/AvtPluginModule.lib
    )
    add_target_additional_deps(${CMAKE_PROJECT_NAME} MORE_FILES
        "${CMAKE_CURRENT_SOURCE_DIR}/sdk/plugin-module/bin/AvtPluginModule.dll"
        # menu-manager should be installed by main plugin
        "${CMAKE_CURRENT_SOURCE_DIR}/sdk/menu-manager/bin/avt-obs-menu-manager.dll"
    )
elseif(APPLE)
    #target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE
    #    ${CMAKE_CURRENT_SOURCE_DIR}/sdk/plugin-module/lib/libAvtPluginModule.dylib
    #)
    target_sources(${CMAKE_PROJECT_NAME} PRIVATE
        src/PluginModuleForMac.cpp
        src/ObsCloseHelper.h
        src/ObsCloseHelper.mm
    )
    target_add_resource(${CMAKE_PROJECT_NAME}
        ${CMAKE_CURRENT_SOURCE_DIR}/sdk/plugin-module/lib/libAvtPluginModule.dylib
    )
elseif(UNIX)
    message("https://cmake.org/cmake/help/latest/variable/UNIX.html?highlight=unix")
endif()
