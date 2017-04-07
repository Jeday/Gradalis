#include "main_head.h"
#include "mainwindow.h"
#include <QApplication>
#include <cassert>





int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QSqlDatabase mydb = QSqlDatabase::addDatabase("QPSQL");
    mainWindow w;
    w.show();

    return a.exec();
}
