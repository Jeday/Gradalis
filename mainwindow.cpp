#include "mainwindow.h"

mainWindow::mainWindow(QWidget *parent)
    : QMainWindow(parent)
{
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
    QPushButton * edit_dialog = new QPushButton(tr("edit_entry"));
    menu_bar_layout->addWidget(edit_dialog);
    QPushButton * edit_dialog1 = new QPushButton(tr("edit_entry1"));
    menu_bar_layout->addWidget(edit_dialog1);
    QPushButton * edit_dialog2 = new QPushButton(tr("edit_entry2"));
    menu_bar_layout->addWidget(edit_dialog2);
    QPushButton * edit_dialog3 = new QPushButton(tr("edit_entry3"));
    menu_bar_layout->addWidget(edit_dialog3);

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

    QVBoxLayout * filter_list_layout = new QVBoxLayout;
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

    filter_area->setLayout(filter_list_layout);
    filter_area->setFrameShape(QFrame::Box);
    filter_area->setMaximumWidth(250);

    /*
       filter area widget
    */

    QScrollArea * scrollarea = new QScrollArea;
    main_box->addWidget(scrollarea);
    //scrollarea->setFixedSize(300,300);

    main_box_top->addWidget(main_box);


}

mainWindow::~mainWindow()
{

}
