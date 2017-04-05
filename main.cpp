#include "main_head.h"
#include "mainwindow.h"
#include <QApplication>
#include <cassert>





int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QSqlDatabase mydb = QSqlDatabase::addDatabase("QPSQL");
    mydb.setHostName("localhost");
    mydb.setDatabaseName("postgres");
    mydb.setUserName("postgres");
    mydb.setPassword("techoo");
    mydb.setPort(5432);




    mainWindow w;
    w.show();

    return a.exec();
}
