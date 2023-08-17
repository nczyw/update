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
#include <QPushButton>
#include <QComboBox>
#include <QGroupBox>
#include <QTimer>
#include <QDebug>

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

    QLabel * labelCurrentVersion = nullptr; //当前软件版本

    QLabel * labelVersionList = nullptr;    //软件版本清单名字
    QComboBox * comboboxVersionList = nullptr;  //版本清单
    QPushButton * btnCheckUpdate = nullptr;     //检查最新版本
    QPushButton * btnGetHistory = nullptr;      //获取历史版本
    QGroupBox * groupVersionInf = nullptr;      //版本相关信息

    QLabel * labelSoftWareName = nullptr;       //软件名字
    QLabel * labelSoftWareVersion = nullptr;    //软件版本
    QLabel * labelSoftWareUpdateTime = nullptr; //更新时间
    QTextEdit * texteditUpdateInf = nullptr;    //更新内容

    QProgressBar * progressbarPercent = nullptr;    //更新进度
    QPushButton * btnStartUpdate = nullptr;         //开始更新按钮

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
    QString softwareversion = "";   //最新软件版本
    QString softwareupdateinf = "";     //更新内容
    QString softwareupdatetime = "";    //软件更新时间
    QString softwareaddress = "";       //软件下载地址

    QSqlDatabase db;
    QString sqlname = "databaseupgrade";

private:

    const QString _address = "--address";       //数据库地址
    const QString _port = "--port";             //数据库端口
    const QString _type = "--type";             //数据库类型
    const QString _databasename = "--database"; //数据库名字
    const QString _username = "--username";     //用户名
    const QString _password = "--password";      //密码
    const QString _tablename = "--tablename";   //表格名字
    const QString _version = "--version";       //软件版本号

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
     * @brief databaseInit  数据库连接
     */
    void databaseInit(void);
    /**
     * @brief checkUpdate   检查更新
     */
    void checkUpdate(void);

    /**
     * @brief getHistroyVersion 获取历史版本
     */
    void getHistroyVersion(void);

    /**
     * @brief changeVersion 改变版本
     */
    void changeVersion(void);

protected:
    void changeEvent(QEvent * e);
};
#endif // UPDATE_H
