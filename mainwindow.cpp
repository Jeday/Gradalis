#include "mainwindow.h"
#include <QtSql>
#include "models.h"


 QSqlTableModel   * clients_model;
 QSqlTableModel * companies_model;
 QSqlTableModel * providers_model;
 QSqlTableModel * manufacturers_model;
 QSqlTableModel * projects_model;

mainWindow::mainWindow(QWidget *parent)
    : QMainWindow(parent)
{

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

    menu->setLayout(menu_bar_layout);
    menu->setFrameShape(QFrame::Box);
    menu->setMaximumHeight(50);

    main_box_top->addWidget(menu);
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
    QComboBox * choose_project = new QComboBox();
    choose_project->setAutoCompletion(true);
    choose_project->setModel(projects_model);
    choose_project->setModelColumn(1);

    filter_list_layout->addRow("Project: ",choose_project);
    QComboBox * choose_company = new QComboBox();
    filter_list_layout->addRow("Company:",choose_company);
    QComboBox * choose_client = new QComboBox();
    filter_list_layout->addRow("Client:",choose_client);
    QComboBox * choose_provider = new QComboBox();
    filter_list_layout->addRow("Provider:",choose_provider);
    QComboBox * choose_manufacturer = new QComboBox();
    filter_list_layout->addRow("Manufacturer:",choose_manufacturer);
    QComboBox * choose_item = new QComboBox();
    filter_list_layout->addRow("Item:",choose_item);







    filter_area->setLayout(filter_list_layout);
    filter_area->setFrameShape(QFrame::Box);
    filter_area->setMaximumWidth(250);

    /*
       filter area widget
    */

    //QScrollArea * scrollarea = new QScrollArea;
   // main_box->addWidget(view);
    //scrollarea->setFixedSize(300,300);
   // scrollarea->setWidget(view);


    main_box_top->addWidget(main_box);

    /*
        init edit tab
    */


    edit_dialog * clients = new edit_dialog(clients_model,"client","RECEIEVER","","grad.checks_out","");
    edit_dialog * providers = new edit_dialog(providers_model,"provider","SENDER","","grad.checks_in","");
    edit_dialog * companies = new edit_dialog(companies_model,"company","RECEIEVER","SENDER","grad.checks_in","grad.checks_out");
    edit_dialog * manufacturers = new edit_dialog(manufacturers_model,"manufacturer","MANUFACTURER","","grad.items","");
    edit_dialog * projects = new edit_dialog(projects_model,"project","PRJ","","grad.checks_in","grad.checks_out");
    editChecks * checks =  new editChecks();
    edit_items * items = new edit_items();
    debug_windget * debg = new debug_windget();

    QTabWidget * edit_tabs = new QTabWidget();
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



}

mainWindow::~mainWindow()
{

}
