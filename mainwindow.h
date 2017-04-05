#ifndef MAINWINDOW_H
#define MAINWINDOW_H


#include "main_head.h"
#include "debug_windget.h"
#include <QtWidgets>
#include <QMainWindow>
#include "editclients.h"
#include "editchecks.h"
#include "edit_dialog.h"
#include "edit_items.h"
#include "models.h"
class mainWindow : public QMainWindow
{
    Q_OBJECT

public:
    mainWindow(QWidget *parent = 0);
    ~mainWindow();
};

#endif // MAINWINDOW_H
