cmake_minimum_required(VERSION 3.5)

project(SII_diagnostics LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# QtCreator supports the following variables for Android, which are identical to qmake Android variables.
# Check https://doc.qt.io/qt/deployment-android.html for more information.
# They need to be set before the find_package( ...) calls below.

#if(ANDROID)
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
#    if (ANDROID_ABI STREQUAL "armeabi-v7a")
#        set(ANDROID_EXTRA_LIBS
#            ${CMAKE_CURRENT_SOURCE_DIR}/path/to/libcrypto.so
#            ${CMAKE_CURRENT_SOURCE_DIR}/path/to/libssl.so)
#    endif()
#endif()

find_package(QT NAMES Qt6 Qt5 COMPONENTS Widgets REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Widgets Core Quick Sql REQUIRED)

set(PROJECT_SOURCES
        main.cpp
        mainwindow.cpp
        mainwindow.h
        mainwindow.ui
        knowledge_editor.cpp
        knowledge_editor.h
        knowledge_editor.ui
        attribute_editor.cpp
        attribute_editor.h
        attribute_editor.ui
        attribute_possible_values.cpp
        attribute_possible_values.h
        attribute_possible_values.ui
        disease_editor.cpp
        disease_editor.h
        disease_editor.ui
        attribute_normal_values.cpp
        attribute_normal_values.h
        attribute_normal_values.ui

)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(SII_diagnostics
        ${PROJECT_SOURCES}
    )
else()
    if(ANDROID)
        add_library(SII_diagnostics SHARED
            ${PROJECT_SOURCES}
        )
    else()
        add_executable(SII_diagnostics
            ${PROJECT_SOURCES}
        )
    endif()
endif()
#find_package(Qt6 REQUIRED COMPONENTS Core Quick Sql)

target_link_libraries(SII_diagnostics PRIVATE
    Qt${QT_VERSION_MAJOR}::Widgets
    Qt${QT_VERSION_MAJOR}::Core
    Qt${QT_VERSION_MAJOR}::Quick
    Qt${QT_VERSION_MAJOR}::Sql)
#set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE "${PROJECT_SOURCE_DIR}/decode.bat")

