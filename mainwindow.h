#ifndef MAINWINDOW_H
#define MAINWINDOW_H

class filter_state;


#include "main_head.h"
#include "debug_windget.h"
#include <QtWidgets>
#include <QMainWindow>
#include "graph_area.h"
#include "edit_dialog.h"
#include "edit_checks.h"
#include "edit_items.h"

//QSqlTableModel * projects_model;


class filter_state:public QObject{

    Q_OBJECT

public:
   filter_state(bool pro,bool cl,bool com,bool pr,bool mn,bool it, QObject * parent = 0 );
   filter_state(bool all,QObject * parent = 0);
   bool is_correct();
   void set_filter_projects(bool r);
   void set_filter_clients(bool r);
   void set_filter_companies(bool r);
   void set_filter_providers(bool r);
   void set_filter_manufacturers(bool r);
   void set_filter_items(bool r);

   bool projects();
   bool companies();
   bool clients();
   bool providers();
   bool items();
   bool manufacturers();

private:
   bool filter_projects;
   bool filter_clients;
   bool filter_companies;
   bool filter_providers;
   bool filter_manufacturers;
   bool filter_items;


private slots:

   void value_changed();

signals:

   void is_valid(bool);

};


class mainWindow : public QMainWindow
{
    Q_OBJECT

public:
    mainWindow(QWidget *parent = 0);
    ~mainWindow();




     filter_state * filter_status;

public slots:
    void filter_update_manufacturer();
     void filter_update();
};

#endif // MAINWINDOW_H
