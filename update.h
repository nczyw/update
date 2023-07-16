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
#include <QLineEdit>

#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlRecord>

class update : public QMainWindow
{
    Q_OBJECT

public:
    update(int argc, char *argv[],QWidget *parent = nullptr);
    ~update();


private:
    //窗体控件
    QLabel * labelTitle = nullptr;      //大标题

    QLabel * labelSoftWareName = nullptr;   //软件名字

    QLabel * labelNowVersion = nullptr; //当前程序版本

    QLabel * labelNewVersion = nullptr; //最新程序版本

    QLabel * labelUpdateTime = nullptr; //更新时间

    QTextEdit * texteditUpdateInf = nullptr;    //更新内容

    QProgressBar * progressbarPercent = nullptr;//进度条

private:
    QString sqladdress = "";        //mysql数据库地址
    int sqlport = 0;                //mysql数据库端口
    QString sqltype = "";           //数据库类型
    QString sqldatabasename= "";    //mysql数据库名字
    QString sqlusername = "";       //mysql用户名
    QString sqlpassword = "";       //mysql密码
    QString sqltablename = "";      //mysql表格名字
    QString softwarecurrentversion = "";    //当前软件版本

    QString softwarename = "";      //要更新的软件名字
    QString softwarelastversion = "";   //最新软件版本
    QString softwareupdateinf = "";     //更新内容
    QString softwareupdatetime = "";    //软件更新时间
    QString softwareaddress = "";       //软件下载地址

    QSqlDatabase db;
    QString sqlname = "databaseupgrade";

private:
    /**
     * @brief configlayout  界面布局
     */
    void configlayout(void);
    /**
     * @brief retranslateUi 界面翻译
     * @param mainwindow
     */
    void retranslateUi(QMainWindow * mainwindow);
    /**
     * @brief checkUpdate   检查更新
     */
    void checkUpdate(void);

protected:
    void changeEvent(QEvent * e);
};
#endif // UPDATE_H
