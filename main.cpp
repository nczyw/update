#include "update.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    update w(argc,argv);
    w.show();
    return a.exec();
}
