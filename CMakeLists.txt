cmake_minimum_required(VERSION 3.5)

project(update VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(${CMAKE_CURRENT_LIST_DIR}/language/languageConfig.cmake)
#qcustomplotConfig.cmake
#include(${CMAKE_CURRENT_LIST_DIR}/qcustomplot/qcustomplotConfig.cmake)

find_package(QT NAMES Qt6 Qt5)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets LinguistTools Sql)




#添加需要支持的语言
set(LanguageFile
    Chinese.ts
    English.ts
)

if(${CMAKE_SYSTEM_NAME} STREQUAL "Windows")
    set(RC version.rc)
    #查找要mysql运行关键文件
    file(GLOB mysqllib ${CMAKE_CURRENT_LIST_DIR}/mysql/*.dll)
    #复制到运行目录
    file(COPY ${mysqllib} DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

    #添加7z相关支持
    file(GLOB 7zlib ${CMAKE_CURRENT_LIST_DIR}/7z/*.*)
    #复制到运行目录
    file(COPY ${7zlib} DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
elseif(${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
    set(RC "")
endif()
set(PROJECT_SOURCES
        ${RC}
        README.md
        main.cpp
        update.cpp
        update.h
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(${PROJECT_NAME}
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET update APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(${PROJECT_NAME} SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(${PROJECT_NAME}
            ${PROJECT_SOURCES}
        )

    endif()
endif()



#配置语言安装目录

if(QT_VERSION_MAJOR EQUAL 6)
    set_source_files_properties(${LanguageFile}
        PROPERTIES OUTPUT_LOCATION "${CMAKE_CURRENT_BINARY_DIR}/language"
        )
    #生成翻译文件。顺便去除失效的翻译，如果已经用Linguist进行翻译去，需要先用Linguist找到失效的翻译，删除翻译内容，才能有效的去除无效翻译
    message("Qt6 language")
    qt_add_translations(${PROJECT_NAME}
        TS_FILES ${LanguageFile}
        LUPDATE_OPTIONS -no-obsolete
        )
#[[
elseif(QT_VERSION_MAJOR EQUAL 5)
    message("Qt5 language")
    qt5_add_translation(${PROJECT_NAME}
        TS_FILES ${LanguageFile}
        )
]]
endif()


target_link_libraries(${PROJECT_NAME} PRIVATE
    Qt${QT_VERSION_MAJOR}::Widgets
    Qt${QT_VERSION_MAJOR}::Sql
    Language_lib
)

set_target_properties(${PROJECT_NAME} PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

install(TARGETS ${PROJECT_NAME}
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(${PROJECT_NAME})
endif()
