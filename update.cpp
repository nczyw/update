#include "update.h"

update::update(int argc, char *argv[], QWidget *parent)
    : QMainWindow(parent)
{

    if(argc != 9){
        QMessageBox::critical(this,tr("错误"),tr("传入参数错误，程序无法运行"));
        qApp->exit(0);
    }
    sqladdress = argv[1];
    sqlport = QString(argv[2]).toInt();
    sqltype = argv[3];
    sqldatabasename = argv[4];
    sqlusername = argv[5];
    sqlpassword = argv[6];
    sqltablename = argv[7];
    softwarecurrentversion = argv[8];
#if 0
    for(int i = 1 ; i < argc ; i += 1){
        qDebug() << argv[i];
    }
#endif
    QFont font;
    font.setFamily("WenQuanYi Micro Hei");
    //font.setFamily("WenQuanYi Micro Hei Mono");
    font.setPointSize(10);
    setFont(font);

    configlayout();

    checkUpdate();




    retranslateUi(this);
}

update::~update()
{
}

void update::configlayout()
{
    labelTitle = new QLabel(this);
    QFont mfont = labelTitle->font();
    mfont.setPointSize(16);
    mfont.setBold(true);
    labelTitle->setFont(mfont);
    labelTitle->setAlignment(Qt::AlignCenter);

    labelSoftWareName = new QLabel(this);

    labelNowVersion = new QLabel(this);

    labelNewVersion = new QLabel(this);

    labelUpdateTime = new QLabel(this);



    texteditUpdateInf = new QTextEdit(this);
    texteditUpdateInf->setReadOnly(true);

    progressbarPercent = new QProgressBar(this);
    progressbarPercent->setAlignment(Qt::AlignCenter);

    progressbarPercent->setValue(0);

    auto vboxall = new QVBoxLayout;
    vboxall->addWidget(labelTitle);
    vboxall->addWidget(labelSoftWareName);
    vboxall->addWidget(labelNowVersion);
    vboxall->addWidget(labelNewVersion);
    vboxall->addWidget(labelUpdateTime);
    vboxall->addWidget(texteditUpdateInf);
    vboxall->addWidget(progressbarPercent);

   // vboxall->addStretch();

    QWidget * widget = new QWidget;
    widget->setLayout(vboxall);
    setCentralWidget(widget);
}

void update::retranslateUi(QMainWindow *mainwindow)
{
    mainwindow->setWindowTitle(tr("自动更新程序"));
    labelTitle->setText(tr("自动更新程序"));
    labelSoftWareName->setText(tr("软件名字：") + "  " + softwarename );
    labelNowVersion->setText(tr("当前程序版本：") + "  " + softwarecurrentversion);
    labelNewVersion->setText(tr("最新程序版本：") + "  " + softwarelastversion );
    labelUpdateTime->setText(tr("更新时间：") + "  " + softwareupdatetime);
    progressbarPercent->setFormat(tr("更新进度：") + "%p%");
}

void update::checkUpdate()
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
    if(db.open()){
        //qDebug() << "数据库打开成功";// << db.tables();

        QSqlQuery query(db);
        QString cmd;
#if 0
        cmd = QString("select * from %1").arg(sqltablename);
        query.exec(cmd);
        for(int i = 0 ; i < query.record().count() ; i += 1){
            auto name = query.record().fieldName(i);
            qDebug() << name;
        }
#endif
#if 0
        qDebug() << sqltablename;
        cmd = QString("select * from %1").arg(sqltablename);
        query.exec(cmd);
        query.first();
        for(int i = 0 ; i < 6 ; i+= 1){
            qDebug() << query.value(i).toString();
        }
#endif \
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
            softwarelastversion = query.value(0).toString();   //获取软件版本
        }

        cmd = QString("select updateinf from %1 where versionattribute='lastversion'").arg(sqltablename);
        if(query.exec(cmd)){
            query.first();
            softwareupdateinf = query.value(0).toString();   //获取更新内容
            texteditUpdateInf->setText(softwareupdateinf);
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
        QMessageBox::critical(this,tr("错误"),tr("数据库连接错误,请检查数据库配置"));

        qApp->exit(0);
    }
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

