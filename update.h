#ifndef UPDATE_H
#define UPDATE_H

#include <QMainWindow>
#include <QEvent>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QDir>
#include <QSettings>
#include <QLabel>
#include <QTextEdit>
#include <QProgressBar>
#include <QCoreApplication>
#include <QMessageBox>

class update : public QMainWindow
{
    Q_OBJECT

public:
    update(QWidget *parent = nullptr);
    ~update();


private:
    //窗体控件
    QLabel * labelTitle = nullptr;      //大标题
    QLabel * labelNowVersion = nullptr; //当前程序版本
    QLabel * labelNewVersion = nullptr; //最新程序版本
    QLabel * labelUpdateTime = nullptr; //更新时间
    QTextEdit * texteditUpdateInf = nullptr;    //更新内容

    QProgressBar * progressbarPercent = nullptr;//进度条

private:
    /**
     * @brief configlayout  界面布局
     */
    void configlayout(void);
    void retranslateUi(QMainWindow * mainwindow);

protected:
    void changeEvent(QEvent * e);
};
#endif // UPDATE_H
