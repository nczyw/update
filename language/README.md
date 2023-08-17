# 多语言支持使用方法
1. 顶层cmake中添加如下语言支持
```
#find_packge 中添加LinguistTools
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
#在target_link_libraries中添加Language_lib
```
```c++
//在要切换语言的widget的头文件中添加如下代码
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
```
2. 在顶层cmake中使用```include(languageConfig.cmake) ``` 或者使用 ```find_packge(language)```
## 使用方法
```c++
   Language().getLanguagelist(); //获取一共有多少语言
   Language().setLanguage("语言"); //设置一种语言
```