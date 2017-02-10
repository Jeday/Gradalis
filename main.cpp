#include "mainwindow.h"
#include <QApplication>
#include <QtSql>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QSqlDatabase db = QSqlDatabase::addDatabase("QPSQL", "my_db");
    db.setHostName("bigblue");
    db.setDatabaseName("flightdb");
    db.setUserName("acarlson");
    db.setPassword("1uTbSbAs");
    //db.
    bool ok = db.open();

    QSqlRelationalTableModel model;
    model.setTable("");
    QTableView *view = new QTableView;
    view->setModel(&model);
    view->show();

    mainWindow w;
    w.show();

    return a.exec();
}
