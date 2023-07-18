#include "update.h"

update::update(int argc, char *argv[], QWidget *parent)
    : QMainWindow(parent)
{

    if(argc != 9 && argc != 2){ //只传递软件版本，使用默认参数工作
        QMessageBox::critical(this,tr("错误"),tr("传入参数错误，程序无法运行"));
        QTimer::singleShot(0,qApp,SLOT(quit()));
    }

    QString str = argv[1];
    //也传递*号时，表示使用默认参数
    if(argc == 2){
        sqladdress = "127.0.0.1";
        sqlport = 3306;
        sqltype = "QMYSQL";
        sqldatabasename ="upgrade";
        sqlusername = "upgrade";
        sqlpassword = "123456";
        sqltablename = "upgrade";
        softwarecurrentversion = argv[2];
    }
    else {
        str == "*" ? sqladdress = "127.0.0.1" : sqladdress = str;

        str = argv[2];
        str == "*" ? sqlport = 3306 : sqlport = str.toInt();

        str = argv[3];
        str == "*" ? sqltype = "QMYSQL" : sqltype = str;

        str = argv[4];
        str == "*" ? sqldatabasename ="upgrade" : sqldatabasename = str;

        str = argv[5];
        str == "*" ? sqlusername = "upgrade" : sqlusername = str ;

        str = argv[6];
        str == "*" ? sqlpassword = "123456" : sqlpassword = str ;

        str = argv[7];
        str == "*" ? sqltablename = "upgrade" : sqltablename = str ;


        softwarecurrentversion = argv[8];
    }
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
            QMessageBox::information(this,tr("提示"),tr("暂未发现新版本"));
        }
    });
    btnGetHistory = new QPushButton(this);

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
        QMessageBox::critical(this,tr("错误"),tr("数据库连接错误,请检查数据库配置"));

        QTimer::singleShot(0,qApp,SLOT(quit()));
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

