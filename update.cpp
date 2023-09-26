#include "update.h"
update::update(int argc, char *argv[], QWidget *parent)
    : QMainWindow(parent)
{
    if((argc - 1) % 2 == 1){
        QMessageBox::critical(this,tr("错误"),tr("传入参数错误，程序无法运行"));
        QTimer::singleShot(0,this,[=]{
            qApp->exit(0);
        });
        return;
    }
    sqladdress = "127.0.0.1";
    sqlport = 3306;
    sqltype = "QMYSQL";
    sqldatabasename ="upgrade";
    sqlusername = "upgrade";
    sqlpassword = "123456";
    sqltablename = "upgrade";
    softwarecurrentversion = "";
    for(int i = 1 ; i < argc ; i += 2){

        QString argi = argv[i];
        QString argp = argv[i + 1];

        if(argi == _address){        //数据库地址
            sqladdress = argp;
        }
        else if(argi == _port){     //数据库端口
            sqlport = argp.toInt();
        }
        else if(argi == _type){     //数据库类型
            sqltype = argp;
        }
        else if(argi == _databasename){     //数据库名字
            sqldatabasename = argp;
        }
        else if(argi == _username){     //用户名
            sqlusername = argp;
        }
        else if(argi == _password){     //密码
            sqlpassword = argp;
        }
        else if(argi == _tablename){     //表格名字
            sqltablename = argp;
        }
        else if(argi == _version){     //程序版本
            softwarecurrentversion = argp;
        }
    }
    if(softwarecurrentversion.trimmed() == ""){ //如果程序版本传递的不对也要退出程序
        QMessageBox::critical(this,tr("错误"),tr("程序版本未传递，程序无法运行"));
        QTimer::singleShot(0,this,[=]{
            qApp->exit(0);
        });
        return;
    }
    /*
    qDebug() << "地址：" << sqladdress;
    qDebug() << "端口：" << sqlport;
    qDebug() << "类型：" << sqltype;
    qDebug() << "数据库：" << sqldatabasename;
    qDebug() << "用户：" << sqlusername;
    qDebug() << "密码：" << sqlpassword;
    qDebug() << "表格：" << sqltablename;
    qDebug() << "版本：" << softwarecurrentversion;
*/
    QFont font;
    font.setFamily("WenQuanYi Micro Hei");
    //font.setFamily("WenQuanYi Micro Hei Mono");
    font.setPointSize(10);
    setFont(font);

    configlayout();

    databaseInit();

    retranslateUi(this);
}

update::~update()
{
    if(db.isOpen()) db.close();
    qDebug() << "更新程序退出";
}

void update::configlayout()
{
    labelTitle = new QLabel(this);
    QFont mfont = labelTitle->font();
    mfont.setPointSize(16);
    mfont.setBold(true);
    labelTitle->setFont(mfont);
    labelTitle->setAlignment(Qt::AlignCenter);
    labelTitle->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Fixed);

    //当前软件版本
    labelCurrentVersion = new QLabel(this);
    labelCurrentVersion->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Fixed);

    //版本管理
    labelVersionList = new QLabel(this);
    comboboxVersionList = new QComboBox(this);
    comboboxVersionList->setSizeAdjustPolicy(QComboBox::AdjustToContents);
 //   comboboxVersionList->setMinimumWidth(150);
    btnCheckUpdate = new QPushButton(this);
    connect(btnCheckUpdate,&QPushButton::clicked,this,[=]{
        checkUpdate();
        retranslateUi(this);
        comboboxVersionList->clear();
        comboboxVersionList->addItem(softwareversion);

        if(softwareversion != softwarecurrentversion){
            btnStartUpdate->setEnabled(true);
            QMessageBox::information(this,tr("提示"),tr("发现新版本：") + softwareversion );
        }
        else {
            btnStartUpdate->setEnabled(false);
            QMessageBox::information(this,tr("提示"),tr("暂未发现新版本"));
        }
    });
    btnGetHistory = new QPushButton(this);
    connect(btnGetHistory,&QPushButton::clicked,this,[=]{
        getHistroyVersion();
    });

    auto hboxManage = new QHBoxLayout;
    hboxManage->addWidget(labelVersionList);
    hboxManage->addWidget(comboboxVersionList);
    hboxManage->addWidget(btnCheckUpdate);
    hboxManage->addWidget(btnGetHistory);
    hboxManage->addStretch();


    groupVersionInf = new QGroupBox(this);  //版本信息
    labelSoftWareName = new QLabel(groupVersionInf);
    labelSoftWareVersion = new QLabel(groupVersionInf);
    labelSoftWareUpdateTime = new QLabel(groupVersionInf);
    texteditUpdateInf = new QTextEdit(this);
    texteditUpdateInf->setReadOnly(true);
    texteditUpdateInf->setLineWrapMode(QTextEdit::NoWrap);
    texteditUpdateInf->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    texteditUpdateInf->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);

    auto vboxVersionInf = new QVBoxLayout;
    vboxVersionInf->addWidget(labelSoftWareName);
    vboxVersionInf->addWidget(labelSoftWareVersion);
    vboxVersionInf->addWidget(labelSoftWareUpdateTime);
    vboxVersionInf->addWidget(texteditUpdateInf);

    groupVersionInf->setLayout(vboxVersionInf);


    //进度条
    progressbarPercent = new QProgressBar(this);
    //progressbarPercent->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    progressbarPercent->setAlignment(Qt::AlignCenter);
    progressbarPercent->setValue(0);
    //progressbarPercent->setValue(50);
    btnStartUpdate = new QPushButton(this);

    auto hboxUpdate = new QHBoxLayout;
    hboxUpdate->addWidget(progressbarPercent);
    hboxUpdate->addWidget(btnStartUpdate);
    btnStartUpdate->setEnabled(false);



    auto vboxAll = new QVBoxLayout;
    vboxAll->addWidget(labelTitle);
    vboxAll->addWidget(labelCurrentVersion);
    vboxAll->addLayout(hboxManage);
    vboxAll->addWidget(groupVersionInf);
    vboxAll->addLayout(hboxUpdate);

    QWidget * widget = new QWidget;
    widget->setLayout(vboxAll);
    setCentralWidget(widget);
}

void update::retranslateUi(QMainWindow *mainwindow)
{
    mainwindow->setWindowTitle(tr("自动更新程序"));
    labelTitle->setText(tr("自动更新程序"));
    labelCurrentVersion->setText(tr("当前软件版本：") + softwarecurrentversion);

    labelVersionList->setText(tr("版本列表："));
    btnCheckUpdate->setText(tr("检查更新"));
    btnGetHistory->setText(tr("历史版本"));

    groupVersionInf->setTitle(tr("版本内容："));
    labelSoftWareName->setText(tr("软件名字：") + "  " + softwarename );
    labelSoftWareVersion->setText(tr("软件版本：") + "  " + softwareversion);
    labelSoftWareUpdateTime->setText(tr("更新时间：") + "  " + softwareupdatetime);

    progressbarPercent->setFormat(tr("更新进度：") + "%p%");
    btnStartUpdate->setText(tr("开始更新"));
}

void update::databaseInit()
{
    if(db.contains(sqlname)){
        db = QSqlDatabase::database(sqlname);
    }
    else {
        db = QSqlDatabase::addDatabase(sqltype,sqlname);
    }

    db.setHostName(sqladdress);
    db.setPort(sqlport);
    db.setDatabaseName(sqldatabasename);
    db.setUserName(sqlusername);
    db.setPassword(sqlpassword);
    if(!db.open()){
        qDebug() << db.lastError().text();
        QMessageBox::critical(this,tr("错误"),tr("数据库连接错误,请检查数据库配置") + "\n" + db.lastError().text());
        QTimer::singleShot(0,qApp,SLOT(quit()));
    }

}

void update::checkUpdate()
{

    if(db.isOpen()){
        //qDebug() << "数据库打开成功";// << db.tables();

        QSqlQuery query(db);
        QString cmd;

    //获取最新的软件版本
        cmd = QString("select name from %1 where versionattribute = 'lastversion'").arg(sqltablename);
        if(query.exec(cmd)){
            query.first();
            softwarename = query.value(0).toString();   //获取软件名字
            // qDebug() <<  "软件名字:" << query.value(0).toString();
        }
        else {
            qDebug() << query.lastError().text();
        }

        cmd = QString("select version from %1 where versionattribute='lastversion'").arg(sqltablename);
        if(query.exec(cmd)){
            query.first();
            softwareversion = query.value(0).toString();   //获取软件版本
        }

        cmd = QString("select updateinf from %1 where versionattribute='lastversion'").arg(sqltablename);
        if(query.exec(cmd)){
            query.first();
            softwareupdateinf = query.value(0).toString();   //获取更新内容
            texteditUpdateInf->setText(softwareupdateinf) ;
        }

        cmd = QString("select updatetime from %1 where versionattribute='lastversion'").arg(sqltablename);
        if(query.exec(cmd)){
            query.first();
            softwareupdatetime = query.value(0).toString();   //获取更新时间
        }

        cmd = QString("select updatetime from %1 where versionattribute='lastversion'").arg(sqltablename);
        if(query.exec(cmd)){
            query.first();
            softwareaddress = query.value(0).toString();   //获取软件存放地址
        }
    }
    else{
        qDebug() << db.lastError().text();
        QMessageBox::critical(this,tr("错误"),tr("更新服务器未连接成功")  + "\n" + db.lastError().text());
        QTimer::singleShot(0,qApp,SLOT(quit()));
    }
}

void update::getHistroyVersion()
{
    comboboxVersionList->clear();
    texteditUpdateInf->clear();
    progressbarPercent->setValue(0);
    btnStartUpdate->setEnabled(false);

    if(db.isOpen()){
        QSqlQuery query(db);
        QString cmd;
        //获取历史版本
        cmd = QString("select version from %1 where versionattribute = 'histroyversion'").arg(sqltablename);
        if(query.exec(cmd)){
            while (query.next()) {  //添加历史版本
                comboboxVersionList->addItem(query.value(0).toString());
            }
        }
        else {
            qDebug() << query.lastError().text();
        }
    }
    else{
        qDebug() << db.lastError().text();
        QMessageBox::critical(this,tr("错误"),tr("更新服务器未连接成功")  + "\n" + db.lastError().text());
        QTimer::singleShot(0,qApp,SLOT(quit()));
    }

}

void update::changeVersion()
{
    progressbarPercent->setValue(0);
    btnStartUpdate->setEnabled(false);

}

void update::changeEvent(QEvent *e)
{
    switch (e->type()) {
    case QEvent::LanguageChange :
        retranslateUi(this);
        break;
    default:
        break;
    }
    QMainWindow::changeEvent(e);
}

