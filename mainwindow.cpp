#include "mainwindow.h"
#include <QtSql>
#include <QSqlQuery>


filter_state::filter_state(bool pro,bool cl,bool com,bool pr,bool mn,bool it,QObject *parent):
    QObject(parent),
    filter_projects(pro),
    filter_clients(cl),
    filter_companies(com),
    filter_items(it),
    filter_providers(pr),
    filter_manufacturers(mn)
    {

}
filter_state::filter_state(bool all,QObject * parent):
    QObject(parent),
    filter_projects(all),
    filter_clients(all),
    filter_companies(all),
    filter_items(all),
    filter_providers(all),
    filter_manufacturers(all)
    {}

bool filter_state::is_correct(){
    return filter_clients&&filter_companies&&filter_items&&filter_manufacturers&&filter_providers&&filter_projects;
}


void filter_state::set_filter_projects(bool r){ filter_projects = r;  value_changed();}
void filter_state::set_filter_clients(bool r){ filter_clients = r; value_changed();}
void filter_state::set_filter_companies(bool r){ filter_companies = r;  value_changed();}
void filter_state::set_filter_providers(bool r){ filter_providers = r; value_changed();}
void filter_state::set_filter_manufacturers(bool r){ filter_manufacturers = r; value_changed();}
void filter_state::set_filter_items(bool r){ filter_items = r; value_changed();}

void filter_state::value_changed(){    emit is_valid(is_correct());}


    bool filter_state::projects(){return filter_projects;}
    bool filter_state::companies(){return filter_companies;}
    bool filter_state::clients(){return filter_clients;}
    bool filter_state::providers(){return filter_providers;}
    bool filter_state::items(){return filter_items;}
    bool filter_state::manufacturers(){return filter_manufacturers;}



    QSqlTableModel * filter_client_model;
    QSqlTableModel * filter_company_model;
    QSqlTableModel * filter_provider_model;
    QSqlTableModel * filter_manufacturer_model;
    QSqlTableModel * filter_item_model;

    QComboBox * choose_project;
    QComboBox * choose_company;
    QComboBox * choose_client;
    QComboBox * choose_provider;
    QComboBox * choose_manufacturer;
    QComboBox * choose_item;

 QSqlTableModel   * clients_model;
 QSqlTableModel * companies_model;
 QSqlTableModel * providers_model;
 QSqlTableModel * manufacturers_model;
 QSqlTableModel * projects_model;

void mainWindow::filter_update(){
    filter_status->blockSignals(true);
    this->blockSignals(true);

    choose_project->blockSignals(true);
    choose_client->blockSignals(true);
    choose_company->blockSignals(true);
    choose_project->blockSignals(true);
    choose_manufacturer->blockSignals(true);
    choose_item->blockSignals(true);


    int project_index = projects_model->index(choose_project->currentIndex(),0).data().toInt();
     //int manufacturer_index = filter_manufacturer_model->index(choose_manufacturer->currentIndex(),0).data().toInt();
    QSqlQuery q;
    //QSqlQuery d;
    QStringList tables;
if (project_index != 1){
    filter_company_model->setFilter("ID IN ((SELECT RECEIEVER FROM grad.checks_in WHERE PRJ = "+QString::number(project_index)+" ) UNION (SELECT SENDER  FROM grad.checks_out WHERE PRJ = "+QString::number(project_index) +" )) OR ID = 1");
//       qDebug()<< d.exec("(SELECT RECEIEVERS IN grad.checks_in WHERE PRJ = "+QString::number(project_index));
//       qDebug()<<d.lastQuery();
//       qDebug()<<d.lastError();
    filter_client_model->setFilter("ID IN  (SELECT RECEIEVER FROM grad.checks_out WHERE PRJ = "+QString::number(project_index)+" ) OR ID = 1");

    filter_provider_model->setFilter("ID IN  (SELECT SENDER FROM grad.checks_in WHERE PRJ = "+QString::number(project_index)+" ) OR ID = 1");
    q.exec("(SELECT ITEM_TABLE FROM grad.checks_in WHERE PRJ = "+QString::number(project_index)+" ) UNION (SELECT ITEM_TABLE  FROM grad.checks_out WHERE PRJ = "+QString::number(project_index) + " )");
    while(q.next()){
            tables.append(q.value("ITEM_TABLE").toString());
        }

    QStringList items;

    foreach (QString table, tables) {
            q.exec("SELECT DISTINCT ITEM_ID FROM "+table);
            while(q.next()){
                    QString item_prototype = q.value("ITEM_ID").toString() ;
                    item_prototype;
                    if (!items.contains(item_prototype))
                        items.append(item_prototype);
                }
        }
    QString items_string;
    items_string = "";
    foreach(QString item, items){
           items_string+=", "+item;
        }
    items_string[0] = ' ';
    items_string;
    filter_manufacturer_model->setFilter("ID IN (SELECT MANUFACTURER FROM grad.items WHERE ID IN ( "+items_string+" )) OR ID = 1");

}
else
    {
        filter_company_model->setFilter("");
        filter_client_model->setFilter("");
        filter_provider_model->setFilter("");
        filter_manufacturer_model->setFilter("");
    }
    filter_company_model->select();
    filter_client_model->select();
    filter_provider_model->select();
    filter_manufacturer_model->select();


    if (choose_manufacturer->currentIndex() < 0)
        choose_manufacturer->setCurrentIndex(0);


        filter_update_manufacturer();

       filter_status->set_filter_clients(filter_client_model->rowCount() > 0);
       filter_status->set_filter_companies(filter_company_model->rowCount() > 0);
       filter_status->set_filter_providers(filter_provider_model->rowCount() > 0);

       filter_status->set_filter_projects(projects_model->rowCount() > 0);

       if (choose_client->currentIndex() < 0)
           choose_client->setCurrentIndex(0);

       if (choose_company->currentIndex() < 0)
           choose_company->setCurrentIndex(0);


       if (choose_provider->currentIndex() < 0)
           choose_provider->setCurrentIndex(0);


       if (choose_project->currentIndex() < 0)
           choose_project->setCurrentIndex(0);



    choose_project->blockSignals(false);
    choose_client->blockSignals(false);
    choose_company->blockSignals(false);
    choose_project->blockSignals(false);
    choose_manufacturer->blockSignals(false);
    choose_item->blockSignals(false);
    this->blockSignals(false);

}

void mainWindow::filter_update_manufacturer(){
    this->blockSignals(true);
    choose_manufacturer->blockSignals(true);
    choose_item->blockSignals(true);

    int manufacturer_index = filter_manufacturer_model->index(choose_manufacturer->currentIndex(),0).data().toInt();
    if(manufacturer_index  != 0)
       filter_item_model->setFilter("MANUFACTURER = " + QString::number(manufacturer_index));
    else
       filter_item_model->setFilter("");
    qDebug()<<filter_item_model->filter();


    filter_item_model->select();


    if (choose_item->currentIndex() < 0)
        choose_item->setCurrentIndex(0);


    filter_status->set_filter_manufacturers(filter_manufacturer_model->rowCount()> 0);
    filter_status->set_filter_items(filter_item_model->rowCount() > 0);

    choose_manufacturer->blockSignals(false);
    choose_item->blockSignals(false);
    this->blockSignals(false);
    filter_status->blockSignals(false);
}

mainWindow::mainWindow(QWidget *parent)
    : QMainWindow(parent)
{

    setWindowIcon(QIcon(":/files/icon_big.ico"));

    clients_model = new QSqlTableModel();
    clients_model->setTable("grad.clients");
    clients_model->select();
    companies_model = new QSqlTableModel();
    companies_model->setTable("grad.companies");
    companies_model->select();
    providers_model = new QSqlTableModel();
    providers_model->setTable("grad.providers");
    providers_model->select();
    manufacturers_model = new QSqlTableModel();
    manufacturers_model->setTable("grad.manufacturers");
    manufacturers_model->select();
    projects_model = new QSqlTableModel();
    projects_model->setTable("grad.projects");
    projects_model->select();



    QWidget* central = new QWidget;
    setCentralWidget(central);
    QBoxLayout * main_lay = new QBoxLayout(QBoxLayout::TopToBottom);

    central->setLayout(main_lay);

    QSplitter * main_box_top = new QSplitter(Qt::Vertical);
    main_lay->addWidget(main_box_top);
    /*
        menu init
    */

    QMenuBar * menu_bar = new QMenuBar();

    QFrame * menu = new QFrame();
    QHBoxLayout * menu_bar_layout = new QHBoxLayout;
    QPushButton * dialog_button0 = new QPushButton(tr("edit data"));
    menu_bar_layout->addWidget(dialog_button0);
    QPushButton * dialog_button1 = new QPushButton(tr("database settings"));
    menu_bar_layout->addWidget(dialog_button1);
    QPushButton * dialog_button2 = new QPushButton(tr("edit_entry2"));
    menu_bar_layout->addWidget(dialog_button2);
    QPushButton * dialog_button3 = new QPushButton(tr("edit_entry3"));
    menu_bar_layout->addWidget(dialog_button3);


    this->setMenuWidget(menu);
    menu->setLayout(menu_bar_layout);
    menu->setFrameShape(QFrame::Box);
    menu->setMaximumHeight(50);

    //main_box_top->addWidget(menu);

    /*
        menu end
    */


    QSplitter * main_box = new QSplitter(Qt::Horizontal);

    /*
        init of filter area widget
    */
    QFrame * filter_area = new QFrame;
    main_box->addWidget(filter_area);


    QFormLayout * filter_list_layout = new QFormLayout;

    filter_status = new filter_state(false);

    choose_project = new QComboBox();
    choose_project->setAutoCompletion(true);
    choose_project->setModel(projects_model);
    choose_project->setModelColumn(1);
    filter_list_layout->addRow("Project: ",choose_project);


    choose_company = new QComboBox();
    filter_company_model = new QSqlTableModel();
    filter_company_model->setTable("grad.companies");
    filter_company_model->select();
    filter_list_layout->addRow("Company:",choose_company);
    choose_company->setModel(filter_company_model);
    choose_company->setModelColumn(1);



    filter_client_model = new QSqlTableModel();
    filter_client_model->setTable("grad.clients");
    filter_client_model->select();
    choose_client = new QComboBox();
    filter_list_layout->addRow("Client:",choose_client);
    choose_client->setModel(filter_client_model);
    choose_client->setModelColumn(1);


    filter_provider_model = new QSqlTableModel();
    filter_provider_model->setTable("grad.providers");
    filter_provider_model->select();
    choose_provider = new QComboBox();
    filter_list_layout->addRow("Provider:",choose_provider);
    choose_provider->setModel(filter_provider_model);
    choose_provider->setModelColumn(1);

    filter_manufacturer_model = new QSqlTableModel();
    filter_manufacturer_model->setTable("grad.manufacturers");
    filter_manufacturer_model->select();
    choose_manufacturer = new QComboBox();
    filter_list_layout->addRow("Manufacturer:",choose_manufacturer);
    choose_manufacturer->setModel(filter_manufacturer_model);
    choose_manufacturer->setModelColumn(1);

    filter_item_model = new QSqlTableModel();
    filter_item_model->setTable("grad.items");
    filter_item_model->select();
    choose_item = new QComboBox();
    filter_list_layout->addRow("Item:",choose_item);
    choose_item->setModel(filter_item_model);
    choose_item->setModelColumn(1);

    connect(choose_project,SIGNAL(currentIndexChanged(int)),this,SLOT(filter_update()));
    connect(choose_manufacturer,SIGNAL(currentIndexChanged(int)),this,SLOT(filter_update_manufacturer()));


    filter_update();

    filter_area->setLayout(filter_list_layout);
    filter_area->setFrameShape(QFrame::Box);
    filter_area->setMaximumWidth(250);

    /*
       filter area widget
    */


    main_box_top->addWidget(main_box);

    /*
        init edit tab
    */


    edit_dialog * clients = new edit_dialog(clients_model,"client","RECEIEVER","","grad.checks_out","");
    edit_dialog * providers = new edit_dialog(providers_model,"provider","SENDER","","grad.checks_in","");
    edit_dialog * companies = new edit_dialog(companies_model,"company","RECEIEVER","SENDER","grad.checks_in","grad.checks_out");
    edit_dialog * manufacturers = new edit_dialog(manufacturers_model,"manufacturer","MANUFACTURER","","grad.items","");
    edit_dialog * projects = new edit_dialog(projects_model,"project","PRJ","","grad.checks_in","grad.checks_out",false);
    editChecks * checks =  new editChecks();
    edit_items * items = new edit_items();
    debug_windget * debg = new debug_windget();

    debg->setWindowIcon(QIcon(":/files/icon_big.ico"));
    debg->setWindowFlags(Qt::Window);

    QTabWidget * edit_tabs = new QTabWidget();
    edit_tabs->setWindowIcon(QIcon(":/files/icon_big.ico"));
    edit_tabs->setWindowFlags(Qt::Window);
    edit_tabs->setWindowTitle("edit");
    edit_tabs->addTab(checks,"checks");
    edit_tabs->addTab(providers,"providers");
    edit_tabs->addTab(companies,"companies");
    edit_tabs->addTab(clients,"clients");
    edit_tabs->addTab(manufacturers,"manufacturers");
    edit_tabs->addTab(items,"items");
    edit_tabs->addTab(projects,"projects");

    QObject::connect(dialog_button0,SIGNAL(clicked(bool)),edit_tabs,SLOT(show()));
    QObject::connect(dialog_button1,SIGNAL(clicked(bool)),debg,SLOT(show()));

    /*
        graph area
    */

    graph_area * garea = new graph_area(filter_status);
    QScrollArea * scrollarea = new QScrollArea();
    scrollarea->setWidget(garea);
    main_box->addWidget(scrollarea);




}

mainWindow::~mainWindow()
{

}
