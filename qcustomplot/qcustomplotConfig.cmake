cmake_minimum_required(VERSION 3.5)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#是否编译成库
set(BuildQcustomPlotLib ON)


include_directories(${CMAKE_CURRENT_LIST_DIR})
include_directories(${CMAKE_CURRENT_LIST_DIR}/inc)
include_directories(${CMAKE_CURRENT_LIST_DIR}/src)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets PrintSupport)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets PrintSupport)
set(QcustomPlot_Headers
        ${CMAKE_CURRENT_LIST_DIR}/inc/qcustomplot.h

    )

set(QcustomPlot_Sources
        ${CMAKE_CURRENT_LIST_DIR}/src/qcustomplot.cpp

    )

set(QcustomPlot_Project
    ${QcustomPlot_Headers}
    ${QcustomPlot_Sources}
    )

#直接像上层传递工程文件
if(BuildQcustomPlotLib)
    #编译成库
    add_library(QcustomPlot_lib
            ${QcustomPlot_Project}
    )
    #给库添加相应依赖
    target_link_libraries(QcustomPlot_lib PRIVATE Qt${QT_VERSION_MAJOR}::Widgets Qt${QT_VERSION_MAJOR}::PrintSupport)
    #在顶层使用target_link_libraries加载该库
endif()

if(CMAKE_COMPILER_IS_GNUCXX)
    # 检查GNU编译是否支持 -Wa,-mbig-obj
    include(CheckCXXCompilerFlag)
    check_cxx_compiler_flag("-Wa,-mbig-obj" GNU_BIG_OBJ_FLAG_ENABLE)
    message(STATUS GNU_BIG_OBJ_FLAG_ENABLE=${GNU_BIG_OBJ_FLAG_ENABLE})
endif()

target_compile_options(QcustomPlot_lib
        PRIVATE
        $<$<CXX_COMPILER_ID:MSVC>:/bigobj>
        $<$<AND:$<CXX_COMPILER_ID:GNU>,$<BOOL:${GNU_BIG_OBJ_FLAG_ENABLE}>>:-Wa,-mbig-obj>
    )
