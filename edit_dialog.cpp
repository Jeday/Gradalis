#include "edit_dialog.h"
#include <QSqlQuery>


void edit_dialog::submit(){
    QSqlQuery q;
    QString s;
    s = "INSERT INTO ";
    s+= model->tableName();
    s+= " (NAME	,DESCRIPTION";
    if (has_lawdesc)
    s+= " ,LAWDESCRIPTION";
    s+= ") ";
    s+= " VALUES ( ";
    s+= " '";
    s+=this->name->text();
    s+= "', '";
    s+=this->desc->text();
    if (has_lawdesc)
    s+="', '"+this->lawdesc->text();
    s+="')";

    q.exec(s);
    model->select();
}


void edit_dialog::delete_sel (){

    foreach (QModelIndex m, view->selectionModel()->selectedIndexes()) {
        int id = model->index(m.row(),0).data().toInt();
        QSqlQuery q;
         q.exec("UPDATE "+ updated_table1 + " SET "+holder_allias1+"= 1 WHERE " + holder_allias1 + " = " + QString::number(id));
        if (this->holder_allias2 != "")
            q.exec("UPDATE "+ updated_table1 + " SET "+holder_allias2+"= 1 WHERE " + holder_allias2 + " = " + QString::number(id));
        if (this->updated_table2 != ""){
            q.exec("UPDATE "+ updated_table2 + " SET "+holder_allias1+"= 1 WHERE " + holder_allias1 + " = " + QString::number(id));
                if (this->holder_allias2 != "")
                     q.exec("UPDATE "+ updated_table2 + " SET "+holder_allias2+"= 1 WHERE " + holder_allias2 + " = " + QString::number(id));

            }
        q.exec("DELETE FROM " +model->tableName()+" WHERE ID = " + QString::number(id));

   }
   model->select();
}

edit_dialog::edit_dialog(QSqlTableModel * _model,QString holder_name,QString _holder_allias1,QString _holder_allias2 ,QString _updated_table1,   QString _updated_table2, bool f,QWidget *parent) :
    QWidget(parent),
    model(_model),
    holder(holder_name),
    updated_table1(_updated_table1),
    updated_table2(_updated_table2),
    holder_allias1(_holder_allias1),
    holder_allias2(_holder_allias2),
    has_lawdesc(f)

{




    QVBoxLayout * main_lay = new QVBoxLayout;
    this->setLayout(main_lay);

    QGroupBox * submit_box = new QGroupBox("new "+holder);

    submit_box->setFlat(false);


    QFormLayout * submit_lay = new QFormLayout;

     name = new QLineEdit;
     desc = new QLineEdit;
     if (has_lawdesc)
     lawdesc = new QLineEdit;
     QPushButton * submit_b= new QPushButton("submit "+holder);

    submit_lay->addRow(holder+" name:",name);
    submit_lay->addRow(" description:",desc);
    if (has_lawdesc)
    submit_lay->addRow(" law descrpition",lawdesc);
    submit_lay->addRow(submit_b);

    submit_box->setLayout(submit_lay);
    main_lay->addWidget(submit_box);


    view = new QTableView();
    view->setModel(model);
    view->hideRow(0);

    QPushButton * delete_selected  = new QPushButton ("delete selected");
    connect(submit_b,SIGNAL(clicked(bool)),this,SLOT(submit()));
    connect(delete_selected,SIGNAL(clicked(bool)),this,SLOT(delete_sel()));
    main_lay->addWidget(delete_selected);
    main_lay->addWidget(view);



}
