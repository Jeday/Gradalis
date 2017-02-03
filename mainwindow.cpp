#include "mainwindow.h"

mainWindow::mainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    QVBoxLayout * lay  = new QVBoxLayout;

    QHBoxLayout * menu_bar_layout  = new QHBoxLayout;
    lay->addLayout(menu_bar_layout);

    QHBoxLayout * main_box_layout = new QHBoxLayout;
    lay->addLayout(main_box_layout);

    QVBoxLayout * filter_list_layout = new QVBoxLayout;
    main_box_layout->addLayout(filter_list_layout);

    QVBoxLayout * view_area_layout = new QVBoxLayout;
    main_box_layout->addLayout(view_area_layout);

    this->setLayout(lay);

    filter_list_layout->addItem(new autoFillComboBox());



}

mainWindow::~mainWindow()
{

}
