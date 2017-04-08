#include "graph_area.h"
#include <models.h>
#include <filter_models.h>
#include <choose_models.h>

qreal Moneytoreal(QString s);

void graph_area::correct_date(){
    end_date->setMaximumDate(QDate::currentDate());
    start_date->setMaximumDate(end_date->date());
}

void graph_area::enable_back(){
    this->setEnabled(true);
}

QDate graph_area::nextDate(QDate d){
    switch (bchart_datetype->currentIndex()) {
        case 1:
            return d.addDays(7*bchart_freq->value());
            break;
        case 2:
            return d.addMonths(bchart_freq->value());
            break;
        case 3:
            return d.addYears(bchart_freq->value());
            break;
        default:
            return d.addDays(bchart_freq->value());
            break;
        }
}

void graph_area::sort_by_company(){


    QString cmp_ind = filter_company_model->index(choose_company->currentIndex(),0).data().toString();

    bool display_all;
    QStringList * cmp_list = new QStringList;
    QStringList * cmp_name_list = new QStringList;
    if(cmp_ind == "1"){
        for(int i = 0; i<filter_company_model->rowCount(); ++i){
                cmp_list->append(filter_company_model->index(i,0).data().toString());
                cmp_name_list->append(filter_company_model->index(i,1).data().toString());
            }
          display_all = true;
        }
    else {
            cmp_list->append(filter_company_model->index(choose_company->currentIndex(),0).data().toString());

            cmp_name_list->append(filter_company_model->index(choose_company->currentIndex(),1).data().toString());
            display_all = false;
        }

    QString item_filter = filter_item_model->index(choose_item->currentIndex(),0).data().toString();
    QString man_filter = filter_manufacturer_model->index(choose_manufacturer->currentIndex(),0).data().toString();
    if (man_filter == "1"){
        if (item_filter == "1")
            man_filter = "";
            else
             man_filter = " WHERE ITEM_ID = " + item_filter + " ";
        }
    else {
            if (item_filter == "1")
             man_filter = " WHERE ITEM_ID IN (SELECT ID grad.items WHERE MANUFACTURER = " + man_filter +" ) ";
            else
             man_filter = " WHERE ITEM_ID = " + item_filter + " ";

    }

    QString clnt_filter = filter_client_model->index(choose_client->currentIndex(),0).data().toString();
    if (clnt_filter == "1")
        clnt_filter = "";
        else
            clnt_filter = " AND RECEIEVER = "+clnt_filter + " ";


    QString pro_fil = providers_model->index(choose_provider->currentIndex(),0).data().toString();

    if (pro_fil == "1")
        pro_fil = "";
        else
            pro_fil = " AND SENDER = "+pro_fil + " ";

    QString prj_fil = projects_model->index(choose_project->currentIndex(),0).data().toString();


    if (prj_fil == "1")
        prj_fil = "";
        else
            prj_fil = " AND PRJ = "+prj_fil + " ";


    QString request = QDate::currentDate().toString("dd'/'MM'/'yyyy");
    request = "http://www.cbr.ru/scripts/XML_dynamic.asp?date_req1="+request+"&date_req2="+request+"&VAL_NM_RQ=R01235";
    QNetworkReply * reply =manager->get(QNetworkRequest(QUrl(request)));
    QEventLoop * l = new QEventLoop();
    this->setDisabled(true);
    connect(reply,SIGNAL(finished()),this,SLOT(enable_back()));
    connect(reply,SIGNAL(finished()),l,SLOT(quit()));
    l->exec();

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

    bchart_values_list = new QList<QList<qreal> *>;

    foreach (QString cmp, *cmp_list) {
        QList<qreal>  * cmp_values = new QList<qreal>();
        QDate date_iter = start_date->date();


        while(date_iter<=end_date->date()){

                QString d1= " '"+date_iter.toString("yyyy'-'MM'-'dd")+ "' ";
                QString d2 = " '"+nextDate(date_iter).toString("yyyy'-'MM'-'dd")+ "' ";

                qreal value = 0;

                QSqlQuery q;
                QString quer="(SELECT ITEM_TABLE,CURR FROM grad.checks_in WHERE PAYED_DATE >= "+d1+" AND PAYED_DATE < "+d2+" AND RECEIEVER = "+cmp+" "+pro_fil+" "+prj_fil+"  ) ";
                quer+= " UNION ALL ";
                quer+="(SELECT ITEM_TABLE,CURR FROM grad.checks_out WHERE PAYED_DATE >= "+d1+" AND PAYED_DATE < "+d2+" AND SENDER= "+cmp+" "+clnt_filter+" "+prj_fil+" ) ";
                q.exec(quer);
                qDebug()<<q.lastError();
                qDebug()<<q.lastQuery();

                    while(q.next()){
                            QString check_table = q.value(0).toString();
                            qreal curr = q.value(1).toDouble();
                            QSqlQuery d;
                            QString dquer = "SELECT PRICE_PER_ITEM, QUNT FROM "+check_table+" "+ man_filter;
                            d.exec(dquer);
                            qDebug()<<d.lastError();
                            qDebug()<<d.lastQuery();
                            qreal sum = 0;
                            while(d.next()){
                                  sum += Moneytoreal(d.value(0).toString())*d.value(1).toInt();
                                }
                            sum*=curr/currency;
                            value+=sum;
                        }

            cmp_values->append(value);
            date_iter = nextDate(date_iter);
        }
       bchart_values_list->append(cmp_values);
      }

    bchart->add(cmp_name_list,bchart_values_list,start_date->date(),end_date->date(),bchart_freq->value(),bchart_datetype->currentText());

    foreach (QList<qreal> * l, *bchart_values_list) {
            delete l;
        }
    delete bchart_values_list;
    bchart_values_list = nullptr;
    delete cmp_name_list;
    cmp_name_list = nullptr;
    delete cmp_list;
    cmp_list = nullptr;
}

void graph_area::sort_by_provider(){
    QString prv_ind = filter_provider_model->index(choose_provider->currentIndex(),0).data().toString();

    bool display_all;
    QStringList * prv_list = new QStringList;
    QStringList * prv_name_list = new QStringList;
    if(prv_ind == "1"){
        for(int i = 0; i<filter_provider_model->rowCount(); ++i){
                prv_list->append(filter_provider_model->index(i,0).data().toString());
                prv_name_list->append(filter_provider_model->index(i,1).data().toString());
            }
          display_all = true;
        }
    else {
            prv_list->append(filter_provider_model->index(choose_provider->currentIndex(),0).data().toString());

            prv_name_list->append(filter_provider_model->index(choose_provider->currentIndex(),1).data().toString());
            display_all = false;
        }

    QString item_filter = filter_item_model->index(choose_item->currentIndex(),0).data().toString();
    QString man_filter = filter_manufacturer_model->index(choose_manufacturer->currentIndex(),0).data().toString();
    if (man_filter == "1"){
        if (item_filter == "1")
            man_filter = "";
            else
             man_filter = " WHERE ITEM_ID = " + item_filter + " ";
        }
    else {
            if (item_filter == "1")
             man_filter = " WHERE ITEM_ID IN (SELECT ID grad.items WHERE MANUFACTURER = " + man_filter +" ) ";
            else
             man_filter = " WHERE ITEM_ID = " + item_filter + " ";

    }


    QString com_fil = filter_company_model->index(choose_company->currentIndex(),0).data().toString();
      if (com_fil == "1"){

        com_fil = "";
        }
        else{

            com_fil = " AND  RECEIEVER = "+com_fil + " ";
        }


      QString prj_fil = projects_model->index(choose_project->currentIndex(),0).data().toString();


      if (prj_fil == "1")
          prj_fil = "";
          else
              prj_fil = " AND PRJ = "+prj_fil + " ";




    QString request = QDate::currentDate().toString("dd'/'MM'/'yyyy");
    request = "http://www.cbr.ru/scripts/XML_dynamic.asp?date_req1="+request+"&date_req2="+request+"&VAL_NM_RQ=R01235";
    QNetworkReply * reply =manager->get(QNetworkRequest(QUrl(request)));
    QEventLoop * l = new QEventLoop();
    this->setDisabled(true);
    connect(reply,SIGNAL(finished()),this,SLOT(enable_back()));
    connect(reply,SIGNAL(finished()),l,SLOT(quit()));
    l->exec();

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

    bchart_values_list = new QList<QList<qreal> *>;

    foreach (QString prv, *prv_list) {
        QList<qreal>  * prv_values = new QList<qreal>();
        QDate date_iter = start_date->date();
        while(date_iter<=end_date->date()){

                QString d1= " '"+date_iter.toString("yyyy'-'MM'-'dd")+ "' ";
                QString d2 = " '"+nextDate(date_iter).toString("yyyy'-'MM'-'dd")+ "' ";

                qreal value = 0;

                QSqlQuery q;
                QString quer="SELECT ITEM_TABLE,CURR FROM grad.checks_in WHERE PAYED_DATE >= "+d1+" AND PAYED_DATE < "+d2+" AND SENDER  = "+prv+" "+com_fil+prj_fil+" ) ";
                q.exec(quer);
                qDebug()<<q.lastError();
                qDebug()<<q.lastQuery();

                    while(q.next()){
                            QString check_table = q.value(0).toString();
                            qreal curr = q.value(1).toDouble();
                            QSqlQuery d;
                            QString dquer = "SELECT PRICE_PER_ITEM, QUNT FROM "+check_table+" "+ man_filter;
                            d.exec(dquer);
                            qDebug()<<d.lastError();
                            qDebug()<<d.lastQuery();
                            qreal sum = 0;
                            while(d.next()){
                                  sum += Moneytoreal(d.value(0).toString())*d.value(1).toInt();
                                }
                            sum*=curr/currency;
                            value+=sum;
                        }

            prv_values->append(value);
            date_iter = nextDate(date_iter);
        }
       bchart_values_list->append(prv_values);
      }

    bchart->add(prv_name_list,bchart_values_list,start_date->date(),end_date->date(),bchart_freq->value(),bchart_datetype->currentText());

    foreach (QList<qreal> * l, *bchart_values_list) {
            delete l;
        }
    delete bchart_values_list;
    bchart_values_list = nullptr;
    delete prv_name_list;
    prv_name_list = nullptr;
    delete prv_list;
    prv_list = nullptr;
}

void graph_area::sort_by_clients(){

    QString cln_ind = filter_client_model->index(choose_client->currentIndex(),0).data().toString();

    bool display_all;
    QStringList * cln_list = new QStringList;
    QStringList * cln_name_list = new QStringList;
    if(cln_ind == "1"){
        for(int i = 0; i<filter_client_model->rowCount(); ++i){
                cln_list->append(filter_client_model->index(i,0).data().toString());
                cln_name_list->append(filter_client_model->index(i,1).data().toString());
            }
          display_all = true;
        }
    else {
            cln_list->append(filter_client_model->index(choose_client->currentIndex(),0).data().toString());

            cln_name_list->append(filter_client_model->index(choose_client->currentIndex(),1).data().toString());
            display_all = false;
        }

    QString item_filter = filter_item_model->index(choose_item->currentIndex(),0).data().toString();
    QString man_filter = filter_manufacturer_model->index(choose_manufacturer->currentIndex(),0).data().toString();
    if (man_filter == "1"){
        if (item_filter == "1")
            man_filter = "";
            else
             man_filter = " WHERE ITEM_ID = " + item_filter + " ";
        }
    else {
            if (item_filter == "1")
             man_filter = " WHERE ITEM_ID IN (SELECT ID grad.items WHERE MANUFACTURER = " + man_filter +" ) ";
            else
             man_filter = " WHERE ITEM_ID = " + item_filter + " ";

    }


    QString com_fil = filter_company_model->index(choose_company->currentIndex(),0).data().toString();
      if (com_fil == "1"){

        com_fil = "";
        }
        else{

            com_fil = " AND  SENDER = "+com_fil + " ";
        }

      QString prj_fil = projects_model->index(choose_project->currentIndex(),0).data().toString();


      if (prj_fil == "1")
          prj_fil = "";
          else
              prj_fil = " AND PRJ = "+prj_fil + " ";




    QString request = QDate::currentDate().toString("dd'/'MM'/'yyyy");
    request = "http://www.cbr.ru/scripts/XML_dynamic.asp?date_req1="+request+"&date_req2="+request+"&VAL_NM_RQ=R01235";
    QNetworkReply * reply =manager->get(QNetworkRequest(QUrl(request)));
    QEventLoop * l = new QEventLoop();
    this->setDisabled(true);
    connect(reply,SIGNAL(finished()),this,SLOT(enable_back()));
    connect(reply,SIGNAL(finished()),l,SLOT(quit()));
    l->exec();

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

    bchart_values_list = new QList<QList<qreal> *>;

    foreach (QString cln, *cln_list) {
        QList<qreal>  * cln_values = new QList<qreal>();
        QDate date_iter = start_date->date();
        while(date_iter<=end_date->date()){

                QString d1= " '"+date_iter.toString("yyyy'-'MM'-'dd")+ "' ";
                QString d2 = " '"+nextDate(date_iter).toString("yyyy'-'MM'-'dd")+ "' ";

                qreal value = 0;

                QSqlQuery q;
                QString quer="SELECT ITEM_TABLE,CURR FROM grad.checks_out WHERE PAYED_DATE >= "+d1+" AND PAYED_DATE < "+d2+" AND RECEIEVER  = "+cln+" "+com_fil+prj_fil+" ) ";
                q.exec(quer);
                qDebug()<<q.lastError();
                qDebug()<<q.lastQuery();

                    while(q.next()){
                            QString check_table = q.value(0).toString();
                            qreal curr = q.value(1).toDouble();
                            QSqlQuery d;
                            QString dquer = "SELECT PRICE_PER_ITEM, QUNT FROM "+check_table+" "+ man_filter;
                            d.exec(dquer);
                            qDebug()<<d.lastError();
                            qDebug()<<d.lastQuery();
                            qreal sum = 0;
                            while(d.next()){
                                  sum += Moneytoreal(d.value(0).toString())*d.value(1).toInt();
                                }
                            sum*=curr/currency;
                            value+=sum;
                        }

            cln_values->append(value);
            date_iter = nextDate(date_iter);
        }
       bchart_values_list->append(cln_values);
      }

    bchart->add(cln_name_list,bchart_values_list,start_date->date(),end_date->date(),bchart_freq->value(),bchart_datetype->currentText());

    foreach (QList<qreal> * l, *bchart_values_list) {
            delete l;
        }
    delete bchart_values_list;
    bchart_values_list = nullptr;
    delete cln_name_list;
    cln_name_list = nullptr;
    delete cln_list;
    cln_list = nullptr;
}


void graph_area::sort_by_projects(){



    QString prj_ind = projects_model->index(choose_project->currentIndex(),0).data().toString();

    bool display_all;
    QStringList * prj_list = new QStringList;
    QStringList * prj_name_list = new QStringList;
    if(prj_ind == "1"){
        for(int i = 0; i<projects_model->rowCount(); ++i){
                prj_list->append(projects_model->index(i,0).data().toString());
                prj_name_list->append(projects_model->index(i,1).data().toString());
            }
          display_all = true;
        }
    else {
            prj_list->append(projects_model->index(choose_project->currentIndex(),0).data().toString());

            prj_name_list->append(projects_model->index(choose_project->currentIndex(),1).data().toString());
            display_all = false;
        }

    QString item_filter = filter_item_model->index(choose_item->currentIndex(),0).data().toString();
    QString man_filter = filter_manufacturer_model->index(choose_manufacturer->currentIndex(),0).data().toString();
    if (man_filter == "1"){
        if (item_filter == "1")
            man_filter = "";
            else
             man_filter = " WHERE ITEM_ID = " + item_filter + " ";
        }
    else {
            if (item_filter == "1")
             man_filter = " WHERE ITEM_ID IN (SELECT ID grad.items WHERE MANUFACTURER = " + man_filter +" ) ";
            else
             man_filter = " WHERE ITEM_ID = " + item_filter + " ";

    }

    QString clnt_filter = filter_client_model->index(choose_client->currentIndex(),0).data().toString();
    if (clnt_filter == "1")
        clnt_filter = "";
        else
            clnt_filter = " AND RECEIEVER = "+clnt_filter + " ";


    QString com_fil = filter_company_model->index(choose_company->currentIndex(),0).data().toString();
    QString com_fil2 = com_fil;
    if (com_fil == "1"){
        com_fil = "";
        com_fil2 = "";
        }
        else{
            com_fil = " AND  RECEIEVER = "+com_fil + " ";
            com_fil2 = " AND  SENDER = "+com_fil2 + " ";
        }

    QString pro_fil = providers_model->index(choose_provider->currentIndex(),0).data().toString();

    if (pro_fil == "1")
        pro_fil = "";
        else
            pro_fil = " AND SENDER = "+pro_fil + " ";



    QString request = QDate::currentDate().toString("dd'/'MM'/'yyyy");
    request = "http://www.cbr.ru/scripts/XML_dynamic.asp?date_req1="+request+"&date_req2="+request+"&VAL_NM_RQ=R01235";
    QNetworkReply * reply =manager->get(QNetworkRequest(QUrl(request)));
    QEventLoop * l = new QEventLoop();
    this->setDisabled(true);
    connect(reply,SIGNAL(finished()),this,SLOT(enable_back()));
    connect(reply,SIGNAL(finished()),l,SLOT(quit()));
    l->exec();

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

    bchart_values_list = new QList<QList<qreal> *>;

    foreach (QString prj, *prj_list) {
        QList<qreal>  * prj_values = new QList<qreal>();
        QDate date_iter = start_date->date();
        qDebug()<<date_iter;
        qDebug()<<start_date->date();
        while(date_iter<=end_date->date()){

                QString d1= " '"+date_iter.toString("yyyy'-'MM'-'dd")+ "' ";
                QString d2 = " '"+nextDate(date_iter).toString("yyyy'-'MM'-'dd")+ "' ";

                qreal value = 0;

                QSqlQuery q;
                QString quer="(SELECT ITEM_TABLE,CURR FROM grad.checks_in WHERE PAYED_DATE >= "+d1+" AND PAYED_DATE < "+d2+" AND PRJ = "+prj+" "+pro_fil+" "+com_fil+" ) ";
                quer+= " UNION ALL ";
                quer+="(SELECT ITEM_TABLE,CURR FROM grad.checks_out WHERE PAYED_DATE >= "+d1+" AND PAYED_DATE < "+d2+" AND PRJ = "+prj+" "+clnt_filter+" "+com_fil2+" ) ";
                q.exec(quer);
                qDebug()<<q.lastError();
                qDebug()<<q.lastQuery();
                qDebug()<<q.at();
                q.next();
                    while(q.next()){
                            qDebug()<<q.at();
                            QString check_table = q.value(0).toString();
                            qreal curr = q.value(1).toDouble();
                            QSqlQuery d;
                            QString dquer = "SELECT PRICE_PER_ITEM, QUNT FROM "+check_table+" "+ man_filter;
                            d.exec(dquer);
                            qreal sum = 0;
                            while(d.next()){
                                    sum += Moneytoreal(d.value(0).toString())*d.value(1).toInt();
                                }
                            sum*=curr/currency;
                            value+=sum;
                        }

            prj_values->append(value);
            date_iter = nextDate(date_iter);
        }
       bchart_values_list->append(prj_values);
      }

    bchart->add(prj_name_list,bchart_values_list,start_date->date(),end_date->date(),bchart_freq->value(),bchart_datetype->currentText());

    foreach (QList<qreal> * l, *bchart_values_list) {
            delete l;
        }
    delete bchart_values_list;
    bchart_values_list = nullptr;
    delete prj_name_list;
    prj_name_list = nullptr;
    delete prj_list;
    prj_list = nullptr;
}


void graph_area::drawgraphs(bool b){
    if (b)
        generate_bchart_data();
}

qreal Moneytoreal(QString s){

    s=s.mid(1);
    qDebug()<<s;
    return s.toDouble();
}

void graph_area::generate_bchart_data(){
    this->blockSignals(true);

    delete chart1;
    delete chart2;

    chart1 = new QChart();
    chart2 = new QChart();
    bchart->stacked_bar_chart=chart1;
    bchart->pie_chart=chart2;

    chartview1->setChart(chart1);
    chartview2->setChart(chart2);

    switch (bchart_sortby->currentIndex()) {
        case 0:
        sort_by_projects();
        break ;
        case 1:
        sort_by_clients();
        break;
        case 2:
        sort_by_provider();
        case 3:
        sort_by_company();
        default:
            break;
        }

    this->blockSignals(false);

}

graph_area::graph_area(filter_state *_filter_status, QWidget *parent):
    filter_status(_filter_status),
    QWidget(parent),
    bchart_values_list(nullptr)
{
    manager = new QNetworkAccessManager();


    bchart_sortby = new QComboBox();
    bchart_sortby->addItems(QStringList()<<"project"<<"client"<<"provider"<<"company");
    bchart_datetype = new QComboBox();
    bchart_datetype->addItems(QStringList()<<"day"<<"week"<<"month"<<"year");
    bchart_freq = new QSpinBox();
    bchart_freq->setMinimum(1);


    chart1 = new QChart();
    chart2 = new QChart();


    bchart = new MyCharts(chart1, chart2);

    chartview1 = new QChartView;
    chartview2 = new QChartView;

    chartview1->setChart(bchart->stacked_bar_chart);
    chartview2->setChart(bchart->pie_chart);

    QVBoxLayout * lay = new QVBoxLayout();
    this->setLayout(lay);
    QHBoxLayout * date_lay = new QHBoxLayout();
    lay->addLayout(date_lay);

    start_date = new QDateEdit(QDate(QDate::currentDate()));
    end_date = new QDateEdit(QDate(QDate::currentDate()));
    connect(end_date,SIGNAL(dateChanged(QDate)),this,SLOT(correct_date()));
    end_date->setDate(QDate::currentDate());
    date_lay->addWidget(start_date);
    date_lay->addWidget(end_date);

    QHBoxLayout * setting_lay = new QHBoxLayout;
    setting_lay->addWidget(bchart_sortby);
    setting_lay->addWidget(bchart_freq);

    lay->addLayout(setting_lay);
    QPushButton * refresh_graphs = new QPushButton("refresh");
    lay->addWidget(refresh_graphs);
    lay->addWidget(chartview1);
    lay->addWidget(chartview2);



    this->setEnabled(true);
    //connect(filter_status,SIGNAL(is_valid(bool)),this,SLOT(setEnabled(bool)));
    //connect(filter_status,SIGNAL(is_valid(bool)),this,SLOT(drawgraphs(bool)));
    connect(refresh_graphs,SIGNAL(clicked(bool)),this,SLOT(generate_bchart_data()));


   // generate_bchart_data();

}


