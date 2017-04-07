#include "edit_items.h"
#include <QFormLayout>
#include <QLayout>
#include <QSqlRelationalTableModel>
#include <QSqlRelationalDelegate>
#include <QSqlResult>
#include "models.h"


void edit_items::submit_item(){
    QSqlQuery q;
    q.exec("INSERT INTO grad.items (NAME,MANUFACTURER) VALUES( '"+name->text()+"' , "+manufacturers_model->index(choose_manufacturers_model->currentIndex(),0).data().toString()+" )");
    qDebug()<<q.lastError();
    item_model->select();

    item_view->resizeColumnsToContents();
    item_view->resizeRowsToContents();
}

void edit_items::del_selected(){
    foreach (QModelIndex m, item_view->selectionModel()->selectedIndexes()){
            QSqlQuery q;
            QString id = item_model->index(m.row(),0).data().toString();
            q.exec("SELECT ITEM_TABLE FROM grad.checks_in UNION SELECT ITEM_TABLE FROM grad.checks_out");
            QStringList table_list;
            while(q.next()){
                    table_list.append(q.value(0).toString());
                }
            foreach (QString table, table_list) {
                    q.exec("DELETE FROM "+table+" WHERE ITEM_ID = " + id);
                }
            q.exec("DELETE FROM "+item_model->tableName()+" WHERE ID = " + id);

        }
        item_model->select();

        item_view->resizeColumnsToContents();
        item_view->resizeRowsToContents();
}

edit_items::edit_items(QWidget *parent) : QWidget(parent)
{
    QVBoxLayout * main_lay = new QVBoxLayout();
    this->setLayout(main_lay);

    QGroupBox * submit_box = new QGroupBox("new item");
    main_lay->addWidget(submit_box);
    QFormLayout * submit_lay = new QFormLayout();
    submit_box->setLayout(submit_lay);


    choose_manufacturers_model = new QComboBox();
    choose_manufacturers_model->setModel(manufacturers_model);
    choose_manufacturers_model->setModelColumn(1);

    name = new QLineEdit();
    QPushButton * submit = new QPushButton("submit item");
    connect(submit,SIGNAL(clicked(bool)),this,SLOT(submit_item()));


    submit_lay->addRow("manufacturer:",choose_manufacturers_model);
    submit_lay->addRow("item name:",name);
    submit_lay->addWidget(submit);





    item_model = new QSqlRelationalTableModel();
    item_model->setTable("grad.items");
    item_model->setRelation(3,QSqlRelation("grad.manufacturers","id","name"));
    item_model->select();
    item_view = new QTableView();
    item_view->setModel(item_model);
    item_view->hideRow(0);
    item_view->hideColumn(0);
    item_model->setEditStrategy(QSqlTableModel::OnFieldChange);
    item_view->setItemDelegate(new QSqlRelationalDelegate(item_view));

    QPushButton * delete_sel = new QPushButton("delete selected");
    connect(delete_sel,SIGNAL(clicked(bool)),this,SLOT(del_selected()));
    main_lay->addWidget(delete_sel);
    main_lay->addWidget(item_view);


    item_model->setHeaderData(1,Qt::Horizontal,tr("Name"));
    item_model->setHeaderData(3,Qt::Horizontal,tr("Manufacturer"));

    item_view->hideColumn(2);
    item_view->resizeColumnsToContents();
    item_view->resizeRowsToContents();


}
