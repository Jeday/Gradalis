#include "edit_checks.h"
#include "models.h"

#include <QTemporaryFile>
#include <QEventLoop>
#include <QXmlStreamReader>
#include <QFile>
#include <QString>

void editChecks::enable_back(){
    this->setEnabled(true);
}


void editChecks::validate_calendars(){
    check_payed_date->setMaximumDate(QDate::currentDate().addDays(1));
    check_pay_date->setMaximumDate(check_payed_date->date());
    check_rec_date->setMaximumDate(check_pay_date->date());

}




void editChecks::manuf_combo_box_clicked(){
    manufacturers_model->select();
}

void editChecks::change_view(){
    if( in_out_checks){
        model->setTable("grad.checks_out");
        model->setRelation(6,QSqlRelation("grad.companies","id","name"));
        model->setRelation(7,QSqlRelation("grad.providers","id","name"));
        model->setRelation(8,QSqlRelation("grad.projects","id","name"));
        model->setHeaderData(6, Qt::Horizontal, tr("Company"));
        model->setHeaderData(7, Qt::Horizontal, tr("Provider"));
        model->setHeaderData(8, Qt::Horizontal, tr("Project"));


        model->select();

        in_out_checks = false;

    }
    else {
        model->setTable("grad.checks_in");
        model->setRelation(6,QSqlRelation("grad.clients","id","name"));
        model->setRelation(7,QSqlRelation("grad.companies","id","name"));
        model->setRelation(8,QSqlRelation("grad.projects","id","name"));

        model->setHeaderData(6, Qt::Horizontal, tr("Client"));
        model->setHeaderData(7, Qt::Horizontal, tr("Company"));
        model->setHeaderData(8, Qt::Horizontal, tr("Project"));

        model->select();
        in_out_checks = true;



    }

    model->setHeaderData(1, Qt::Horizontal, tr("Check ID"));
    model->setHeaderData(2, Qt::Horizontal, tr("Description"));
    model->setHeaderData(3, Qt::Horizontal, tr("Rec. Date"));
    model->setHeaderData(4, Qt::Horizontal, tr("Pay Date"));
    model->setHeaderData(5, Qt::Horizontal, tr("Payed Date"));

    view->hideRow(0);
     view->resizeColumnsToContents();
     view->resizeRowsToContents();
     view->hideRow(0);
     view->hideColumn(0);
     view->hideColumn(9);
     view->hideColumn(10);


}


void editChecks::in_out_changed_new_check(bool t){
    if (t){
        senders->setModel(providers_model);
        senders->setModelColumn(1);
        senders->setCurrentIndex(0);

        reciv->setModel(companies_model);
        reciv->setModelColumn(1);
        reciv->setCurrentIndex(0);

        send_label->setText("provider: ");
        recv_label->setText("company: ");


    }
    else {


        senders->setModel(companies_model);
        senders->setModelColumn(1);
        senders->setCurrentIndex(0);

        reciv->setModel(clients_model);
        reciv->setModelColumn(1);
        reciv->setCurrentIndex(0);

        send_label->setText("company: ");
        recv_label->setText("client: ");


    }

}

void editChecks::change_item_model(QModelIndex m){
     item_model->setTable(model->index(m.row(),9).data().toString());
     item_model->setRelation(1,QSqlRelation("grad.items","id","name"));
     item_model->select();
     item_model->setHeaderData(1, Qt::Horizontal, tr("Name"));
     item_model->setHeaderData(2, Qt::Horizontal, tr("Quntaty"));
     item_model->setHeaderData(3, Qt::Horizontal, tr("Price"));

     item_view->hideColumn(0);
     item_view->setItemDelegate(new QSqlRelationalDelegate(view));
     item_view->resizeColumnsToContents();
     item_view->resizeRowsToContents();

     if (item_model->tableName() == ""){
             new_item->setDisabled(true);
         }
     else
         new_item->setEnabled(true);

}

void editChecks::sumbit_check(){



    QString date_rec =" '"+check_rec_date->date().toString("yyyy'-'MM'-'dd")+"' ";
    QString date_pay = " '"+check_pay_date->date().toString("yyyy'-'MM'-'dd")+"' ";
    QString date_payed = " '"+check_payed_date->date().toString("yyyy'-'MM'-'dd")+"' ";

        QString request = check_rec_date->date().toString("dd'/'MM'/'yyyy");
        request = "http://www.cbr.ru/scripts/XML_dynamic.asp?date_req1="+request+"&date_req2="+request+"&VAL_NM_RQ=R01235";
        QNetworkReply * reply =manager->get(QNetworkRequest(QUrl(request)));

        QEventLoop l;
        this->setDisabled(true);
        connect(reply,SIGNAL(finished()),this,SLOT(enable_back()));
        connect(reply,SIGNAL(finished()),&l,SLOT(quit()));
        l.exec();

        //reply->setTextModeEnabled(true);

        qreal currency;
        if(reply->error() == QNetworkReply::NoError)
            {
                QString s = reply->readAll();
                int i = s.indexOf("<Value>",0);
                i = s.indexOf(">",i);

                s = s.mid(i+1,s.indexOf("<",i)-i-1);
                    s.replace(',',".");
                    qDebug()<<s;
                    currency = s.toFloat();
                    qDebug()<<currency;
                }



        QString v= "VALUES ('"+check_name->text()+"', '"+check_desc->text()+"', "+date_rec+", "+date_pay+", "+date_payed+")";
        QSqlQuery query;
        if (in_check->isChecked())
        query.exec("INSERT INTO grad.checks_in (NAME, DESCRIPTION,REC_DATE,PAY_DATE,PAYED_DATE) "
                         +v+ "RETURNING id");
        else
            query.exec("INSERT INTO grad.checks_out  (NAME, DESCRIPTION,REC_DATE,PAY_DATE,PAYED_DATE) "
                             +v+ "RETURNING ID");
        //qDebug()<<query.lastError();
        query.next();
        QString id =query.value(0).toString();

        QString s,s_no_sch;

        if (in_check->isChecked()){
            s = "grad.in_checks_item_"+id;
            s_no_sch ="in_checks_item_"+id ;
            }
        else{
             s = "grad.out_checks_item_"+id;
             s_no_sch ="out_checks_item_"+id;
            }
        QString _s =    "CREATE TABLE " + s +
        " (  id bigserial NOT NULL,      item_id integer NOT NULL, qunt integer NOT NULL,  price_per_item money NOT NULL, CONSTRAINT "+s_no_sch+"_key PRIMARY KEY (id),"+
        "  CONSTRAINT "+s_no_sch+"_item_id_fkey FOREIGN KEY (item_id)        REFERENCES grad.items (id) MATCH SIMPLE         ON UPDATE NO ACTION ON DELETE NO ACTION   )"+
        "  WITH ( OIDS=FALSE );  ALTER TABLE " + s +   "  OWNER TO postgres;";



       query.exec(_s);

        QString rec_id = reciv->model()->index(reciv->currentIndex(),0).data().toString();
        QString send_id = senders->model()->index(senders->currentIndex(),0).data().toString();
        QString prj_id = projects_model->index(projects->currentIndex(),0).data().toString();

        if (in_check->isChecked())
         _s = "UPDATE grad.checks_in SET ITEM_TABLE = '"+s+"' WHERE ID = "+id;
        else
           _s = "UPDATE grad.checks_out SET ITEM_TABLE = '"+s+"' WHERE ID = "+id;
        query.exec(_s);



        if (in_check->isChecked())
         _s = "UPDATE grad.checks_in SET SENDER = '"+send_id+"' WHERE ID = "+id;
        else
           _s = "UPDATE grad.checks_out SET SENDER = '"+send_id+"' WHERE ID = "+id;
        query.exec(_s);


        if (in_check->isChecked())
         _s = "UPDATE grad.checks_in SET RECEIEVER = '"+rec_id+"' WHERE ID = "+id;
        else
         _s = "UPDATE grad.checks_out SET RECEIEVER = '"+rec_id+"' WHERE ID = "+id;
        query.exec(_s);


        if (in_check->isChecked())
         _s = "UPDATE grad.checks_in SET PRJ = "+prj_id+" WHERE ID = "+id;
        else
         _s = "UPDATE grad.checks_out SET PRJ = "+prj_id+" WHERE ID = "+id;
        query.exec(_s);

        if (in_check->isChecked())
         _s = "UPDATE grad.checks_in SET CURR = "+QString::number(currency)+" WHERE ID = "+id;
        else
         _s = "UPDATE grad.checks_out SET CURR = "+QString::number(currency)+" WHERE ID = "+id;
        query.exec(_s);
       // qDebug()<<query.lastError();
        //qDebug()<<query.lastError();

        model->select();

    }

    void editChecks::del_selected(){
        //qDebug()<<"in del";
        foreach (QModelIndex v, view->selectionModel()->selectedIndexes()) {
            QSqlQuery q;
            QString s;
            s= "DROP TABLE IF EXISTS "+model->index(v.row(),9).data().toString();
            q.exec(s);
            model->removeRow(v.row());
            view->setModel(model);
            item_view->setModel(item_model);
        }

        model->select();


    }

    void editChecks::add_item(){

        QString s = "INSERT INTO "+item_model->tableName()+ " (ITEM_ID,QUNT,PRICE_PER_ITEM) VALUES( ";
        s+=item_list_model->index(check_choose_item->currentIndex(),0).data().toString();

        s+=" , ";

        s+=  QString::number(item_qunt->value());

        s+=" , ";

        s+= QString::number(item_price->value());

        s+=" )";



        QSqlQuery q;
        q.exec(s);

        item_model->select();

    }

    void editChecks::change_item_filer(int ind){
        item_list_model->setFilter("manufacturer = "+ manufacturers_model->index(choose_manuf->currentIndex(),0).data().toString());
        item_list_model->select();
        if (item_list_model->rowCount() == 0)
            new_item->setDisabled(true);
        else
            new_item->setDisabled(false);

        //check_choose_item->setCurrentIndex(0);
}


void editChecks::del_selected_item(){
    foreach (QModelIndex v, item_view->selectionModel()->selectedIndexes()){

        item_model->removeRow(v.row());


        }

    item_model->select();

}

editChecks::editChecks(QWidget *parent) : QWidget(parent),  in_out_checks(true)
{


    manager = new QNetworkAccessManager(this);
    manager->setNetworkAccessible(QNetworkAccessManager::Accessible);

    QVBoxLayout * lay = new QVBoxLayout();
    this->setLayout(lay);

    /*
     gruop box for new check
    */

    QGroupBox * _box = new QGroupBox(tr("new check"));
    lay->addWidget(_box);

    /*
     in_out checks
    */

    in_check= new QRadioButton("in check");
    in_check->setChecked(in_out_checks);



    /*
     lineedits and buttons for new check
    */

     check_name = new QLineEdit();
     check_desc = new QLineEdit();
     check_rec_date = new QDateEdit(QDate::currentDate());
     check_pay_date = new QDateEdit(QDate::currentDate());
     check_payed_date = new QDateEdit(QDate::currentDate());
     connect(check_rec_date,SIGNAL(dateChanged(QDate)),this,SLOT(validate_calendars()));
     connect(check_pay_date,SIGNAL(dateChanged(QDate)),this,SLOT(validate_calendars()));
     connect(check_payed_date,SIGNAL(dateChanged(QDate)),this,SLOT(validate_calendars()));
     check_rec_date->setCalendarPopup(true);
     check_pay_date->setCalendarPopup(true);
     check_payed_date->setCalendarPopup(true);


     add_check = new QPushButton("SUMBIT CHECK");

     /*
      senders and rec
     */


     send_label = new QLabel("provider: ");
     senders = new QComboBox();
     senders->setModel(providers_model);
     senders->setModelColumn(1);


     recv_label = new QLabel("company: ");
     reciv = new QComboBox();
     reciv->setModel(companies_model);
     reciv->setModelColumn(1);

     proj_label = new QLabel("project: ");
     projects = new QComboBox();
     projects->setModel(projects_model);
     projects->setModelColumn(1);

     QVBoxLayout * box_lay = new QVBoxLayout;
     QHBoxLayout * box = new QHBoxLayout;
     _box->setLayout(box_lay);
     box->addWidget(in_check);

     QFormLayout * line1 = new QFormLayout;
     line1->addRow(new QLabel("check ID:"),check_name);
     line1->setRowWrapPolicy(QFormLayout::WrapAllRows);

     QFormLayout * line2 = new QFormLayout;
     line2->addRow(new QLabel("check description:"),check_desc);
     line2->setRowWrapPolicy(QFormLayout::WrapAllRows);

     box->addLayout(line1);
     box->addLayout(line2);
     box_lay->addLayout(box);

     QVBoxLayout * hlay = new QVBoxLayout;
     QHBoxLayout * vlay = new QHBoxLayout;

     QFormLayout * column1 = new QFormLayout;
     column1->addRow(send_label,senders);
     column1->addRow(recv_label,reciv);
     column1->addRow(proj_label,projects);
     column1->setRowWrapPolicy(QFormLayout::WrapAllRows);
     vlay->addLayout(column1);

     QFormLayout * column2 = new QFormLayout;
     column2->addRow("check recieve date:",check_rec_date);
     column2->addRow("check pay date:",check_pay_date);
     column2->addRow("check payed date:",check_payed_date);
     column2->setRowWrapPolicy(QFormLayout::WrapAllRows);
     vlay->addLayout(column2);

     hlay->addLayout(vlay);
     box_lay->addLayout(hlay);
     box_lay->addWidget(add_check);
     _box->setMinimumHeight(300);

    QObject::connect(in_check,SIGNAL(clicked(bool)),this,SLOT(in_out_changed_new_check(bool)));
    QObject::connect(add_check,SIGNAL(clicked(bool)),this,SLOT(sumbit_check()));

    /*
        init model  and view for checks table
    */
    model = new QSqlRelationalTableModel(this,QSqlDatabase::database());

    model->setTable("grad.checks_in");
    model->setHeaderData(1, Qt::Horizontal, tr("Check ID"));
    model->setHeaderData(2, Qt::Horizontal, tr("Description"));
    model->setHeaderData(3, Qt::Horizontal, tr("Rec. Date"));
    model->setHeaderData(4, Qt::Horizontal, tr("Pay Date"));
    model->setHeaderData(5, Qt::Horizontal, tr("Payed Date"));

    model->setHeaderData(6, Qt::Horizontal, tr("Company"));
    model->setHeaderData(7, Qt::Horizontal, tr("Provider"));
    model->setHeaderData(8, Qt::Horizontal, tr("Project"));
    model->setRelation(6,QSqlRelation("grad.companies","id","name"));
    model->setRelation(7,QSqlRelation("grad.providers","id","name"));
    model->setRelation(8,QSqlRelation("grad.projects","id","name"));
    model->setEditStrategy(QSqlTableModel::OnFieldChange);
    model->select();


    view = new QTableView();
    view->setModel(model);
    view->setItemDelegate(new QSqlRelationalDelegate(view));
    view->setSortingEnabled(true);
    view->setWordWrap(false);

    view->hideRow(0);
    view->hideColumn(0);
    view->hideColumn(9);
    view->hideColumn(10);



    /*
       init buttons and connect for INOUT chekcs display and delete selected
    */

    QHBoxLayout * button_lay = new QHBoxLayout();
    QPushButton * in_out  = new QPushButton("IN/OUT CHECKS");
    QPushButton * delete_selected = new QPushButton("DELETE SELECTED");
    button_lay->addWidget(in_out);
    button_lay->addWidget(delete_selected);

    QObject::connect(delete_selected,SIGNAL(clicked(bool)),this,SLOT(del_selected()));
    QObject::connect(in_out,SIGNAL(clicked(bool)),this,SLOT(change_view()));


    /*
        add to main layout buttons lay and view
    */
    lay->addLayout(button_lay);
    lay->addWidget(view);



    /*
      item  model and view  init, connect check selection and item liust displayed
    */

    item_model = new QSqlRelationalTableModel(nullptr, QSqlDatabase::database());
    item_model->setEditStrategy(QSqlTableModel::OnFieldChange);
    item_view = new QTableView();
    item_view->setModel(item_model);
    item_view->setSortingEnabled(true);
    item_view->hideColumn(0);
    item_view->setItemDelegate(new QSqlRelationalDelegate(view));


    /*
      new item box
    */

    QGroupBox *new_item_box = new QGroupBox("new item");
    QVBoxLayout *new_item_lay_vert = new QVBoxLayout;
    QHBoxLayout* new_item_lay = new QHBoxLayout;
    new_item_box->setLayout(new_item_lay_vert);
    new_item_lay_vert->addLayout(new_item_lay);


    choose_manuf = new QComboBox;
    choose_manuf->setModel(manufacturers_model);
    choose_manuf->setModelColumn(1);
    choose_manuf->setCurrentIndex(0);


    item_list_model = new QSqlRelationalTableModel;
    item_list_model->setTable("grad.items");
    item_list_model->setFilter("manufacturer = "+ manufacturers_model->index(choose_manuf->currentIndex(),0).data().toString());
    item_list_model->select();
    //qDebug()<<manufacturers_model->index(choose_manuf->currentIndex(),0).data().toString();
    QObject::connect(choose_manuf,SIGNAL(currentIndexChanged(int)),this,SLOT(change_item_filer(int)));
    //QObject::connect(choose_manuf,SIGNAL(activated(int)),this,SLOT(manuf_combo_box_clicked()));





    check_choose_item = new QComboBox;
    check_choose_item->setModel(item_list_model);
    check_choose_item->setModelColumn(1);
    check_choose_item->setCurrentIndex(0);

    item_qunt = new QSpinBox();
    item_qunt->setMinimum(0);
    item_qunt->setMaximum(10000000);
    item_qunt->setValue(0);
    item_qunt->setSingleStep(1);

    item_price = new QDoubleSpinBox();
    item_price->setMinimum(0.0);
    item_price->setMaximum(10000000000);
    item_price->setValue(0.0);

    new_item_lay->addWidget(choose_manuf);
    new_item_lay->addWidget(check_choose_item);
    new_item_lay->addWidget(item_qunt);
    new_item_lay->addWidget(item_price);



    new_item = new QPushButton("submit item");
    new_item->setDisabled(true);

    new_item_lay_vert->addWidget(new_item);
    QObject::connect(new_item,SIGNAL(clicked(bool)),this,SLOT(add_item()));





    lay->addWidget(new_item_box);



    QPushButton * delete_selected_item = new QPushButton("delete selected item");
    QObject::connect(delete_selected_item,SIGNAL(clicked(bool)),this,SLOT(del_selected_item()));
    lay->addWidget(delete_selected_item);


    lay->addWidget(item_view);
    QObject::connect(view,SIGNAL(clicked(QModelIndex)),this,SLOT(change_item_model(QModelIndex)));



}



