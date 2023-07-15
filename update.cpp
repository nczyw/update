#include "update.h"

update::update(QWidget *parent)
    : QMainWindow(parent)
{

    QFont font;
    font.setFamily("WenQuanYi Micro Hei");
    //font.setFamily("WenQuanYi Micro Hei Mono");
    font.setPointSize(10);
    setFont(font);

    configlayout();

    retranslateUi(this);

    QFile file(QCoreApplication::applicationDirPath() + "/update/updateinf.txt");
    if(file.open(QIODevice::ReadOnly | QIODevice::Text)){
        QByteArray text = file.readAll();
        texteditUpdateInf->setText(text);
        file.close();
    }
    else{
        QMessageBox::critical(this,tr("错误"),tr("未发现准备就绪的更新文件，点击确定后退出"));
    }
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
    vboxall->addWidget(labelNowVersion);
    vboxall->addWidget(labelNewVersion);
    vboxall->addWidget(labelUpdateTime);
    vboxall->addWidget(texteditUpdateInf);
    vboxall->addWidget(progressbarPercent);

    vboxall->addStretch();

    QWidget * widget = new QWidget;
    widget->setLayout(vboxall);
    setCentralWidget(widget);
}

void update::retranslateUi(QMainWindow *mainwindow)
{
    mainwindow->setWindowTitle(tr("自动更新程序"));
    labelTitle->setText(tr("自动更新程序"));
    labelNowVersion->setText(tr("当前程序版本：") + "1.0.0.0");
    labelNewVersion->setText(tr("最新程序版本：") + "1.0.0.1");
    labelUpdateTime->setText(tr("更新时间：") + "202307141521");
    progressbarPercent->setFormat(tr("更新进度：") + "%p%");
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

