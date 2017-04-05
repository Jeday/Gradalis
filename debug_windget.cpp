#include "debug_windget.h"
#include <QString>
#include <QFile>
#include <QTextStream>
#include <QSqlQuery>


void debug_windget::init_bd(){
    QFile * f = new QFile(":/files/quer.sql");
    f->open(QFile::ReadOnly);
    QTextStream t(f);
    QSqlQuery q;
    qDebug()<<q.exec(t.readAll());
    qDebug()<<q.lastError();
}


debug_windget::debug_windget(QWidget *parent) : QWidget(parent)
{
    QVBoxLayout * main_lay = new QVBoxLayout;
    this->setLayout(main_lay);


    QGroupBox * box  = new QGroupBox("database settings");
    main_lay->addWidget(box);

    QVBoxLayout * lay = new QVBoxLayout;
    box->setLayout(lay);



    lay->addWidget(new QLabel("set hostname:"));
    host_name = new QLineEdit("localhost");
    lay->addWidget(host_name);

    lay->addWidget(new QLabel("set database name:"));
    db_name = new QLineEdit("postgres");
    lay->addWidget(db_name);


    lay->addWidget(new QLabel("set user name:"));
    user_name = new QLineEdit("postgres");
    lay->addWidget(user_name);


    lay->addWidget(new QLabel("set password:"));
    password = new QLineEdit("techoo");
    password->setEchoMode(QLineEdit::Password);
    lay->addWidget(password);


    lay->addWidget(new QLabel("set port:"));
    port = new QLineEdit("5432");
    lay->addWidget(port);


    status = new QLabel();
    if (QSqlDatabase::database().isOpen())
        status->setText("database connected: true");
    else
        status->setText("database connected: false");

    QPushButton * cont = new QPushButton("connect to database");
    lay->addWidget(cont);

    connect(cont,SIGNAL(clicked(bool)),this,SLOT(connect_to_database()));

    lay->addWidget(status);

    QPushButton * init_bd = new QPushButton("initialise database");
    lay->addWidget(init_bd);

    connect(init_bd,SIGNAL(clicked(bool)),this,SLOT(init_bd()));
}


void debug_windget::connect_to_database(){
    QSqlDatabase db = QSqlDatabase::database();
    db.setHostName(host_name->text());
    db.setDatabaseName(db_name->text());
    db.setUserName(user_name->text());
    db.setPassword(password->text());
    db.setPort(port->text().toInt());
    if (!db.open())
        status->setText("database connected: connection error");
    else
         status->setText("database connected: true");
}
