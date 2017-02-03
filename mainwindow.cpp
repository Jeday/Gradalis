#include "mainwindow.h"

mainWindow::mainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    QWidget* central = new QWidget;
    setCentralWidget(central);
    QVBoxLayout* lay  = new QVBoxLayout;

    QHBoxLayout * menu_bar_layout  = new QHBoxLayout;
    lay->addLayout(menu_bar_layout);

    QPushButton * edit_dialog = new QPushButton(tr("edit_entry"));
    menu_bar_layout->addWidget(edit_dialog);
    QPushButton * edit_dialog1 = new QPushButton(tr("edit_entry1"));
    menu_bar_layout->addWidget(edit_dialog1);
    QPushButton * edit_dialog2 = new QPushButton(tr("edit_entry2"));
    menu_bar_layout->addWidget(edit_dialog2);
    QPushButton * edit_dialog3 = new QPushButton(tr("edit_entry3"));
    menu_bar_layout->addWidget(edit_dialog3);


    QHBoxLayout * main_box_layout = new QHBoxLayout;
    lay->addLayout(main_box_layout);

    QVBoxLayout * filter_list_layout = new QVBoxLayout;
    main_box_layout->addLayout(filter_list_layout);

    autoFillComboBox * fil_1 = new autoFillComboBox();
    filter_list_layout->addWidget(fil_1);
    autoFillComboBox * fil_2 = new autoFillComboBox();
    filter_list_layout->addWidget(fil_2);
    autoFillComboBox * fil_3 = new autoFillComboBox();
    filter_list_layout->addWidget(fil_3);
    autoFillComboBox * fil_4 = new autoFillComboBox();
    filter_list_layout->addWidget(fil_4);
    autoFillComboBox * fil_5 = new autoFillComboBox();
    filter_list_layout->addWidget(fil_5);





    QVBoxLayout * view_area_layout = new QVBoxLayout;
    main_box_layout->addLayout(view_area_layout);

    QScrollArea * scrollarea = new QScrollArea;
    view_area_layout->addWidget(scrollarea);
    scrollarea->setFixedSize(300,300);





    central->setLayout(lay);

}

mainWindow::~mainWindow()
{

}
