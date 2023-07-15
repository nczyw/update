cmake_minimum_required(VERSION 3.5)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#是否编译成库
set(BuildLanguageLib ON)


include_directories(${CMAKE_CURRENT_LIST_DIR})
include_directories(${CMAKE_CURRENT_LIST_DIR}/inc)
include_directories(${CMAKE_CURRENT_LIST_DIR}/src)

set(Language_Headers
        ${CMAKE_CURRENT_LIST_DIR}/inc/language.h

    )

set(Language_Sources
        ${CMAKE_CURRENT_LIST_DIR}/src/language.cpp

    )

set(Language_Project
    ${Language_Headers}
    ${Language_Sources}
    )

#直接像上层传递工程文件
if(BuildLanguageLib)
    #编译成库
    add_library(Language_lib
            ${Language_Project}
    )
    #给库添加相应依赖
    target_link_libraries(Language_lib PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)
    #在顶层使用target_link_libraries加载该库
endif()




#[[

#下面是Cmake处理
在find_package()中添加语言  LinguistTools

#添加需要支持的语言
set(LanguageFile
    中文.ts
    English.ts
)

#配置语言安装目录
set_source_files_properties(${LanguageFile}
    PROPERTIES OUTPUT_LOCATION "${CMAKE_CURRENT_BINARY_DIR}/language"
    )
#生成翻译文件。顺便去除失效的翻译，如果已经用Linguist进行翻译了，需要先用Linguist找到失效的翻译，删除翻译内容，才能有效的去除无效翻译
qt_add_translations(${PROJECT_NAME}
    TS_FILES ${LanguageFile}
    LUPDATE_OPTIONS -no-obsolete
    )

#语言切换关键代码
//在头文件中添加
protected:
    void changeEvent(QEvent * e);

    //在cpp文件中 changeEvent(QEvent * e)函数中添加如下代码
    //retranslateUi函数使用ui界面构建时，会自动创建，手工创建的界面，要手动完成retranslateUi函数，
    switch (e->type()) {
    case QEvent::LanguageChange :
        retranslateUi(this);
        break;
    default:
        break;
    }
    QMainWindow::changeEvent(e);

]]

