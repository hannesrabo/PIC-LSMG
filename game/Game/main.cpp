#include "mainwindow.h"
#include <QApplication>
#include <QProcess>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    //Launching our program
    QString program = "spi";
    QProcess *p = new QProcess(this);

    p->start(program);

    return a.exec();
}
