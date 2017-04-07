#ifndef EDIT_DIALOG_H
#define EDIT_DIALOG_H

#include "main_head.h"
#include <QWidget>
#include <QtWidgets>
#include <QLayout>



class edit_dialog : public QWidget
    {
    Q_OBJECT

private:

    QSqlTableModel * model;
    QTableView * view;
    QString holder;
    QString updated_table1;
    QString updated_table2;
    QString holder_allias1;
    QString holder_allias2;

    QLineEdit * name ;
    QLineEdit * desc ;
    QLineEdit * lawdesc;

    bool has_lawdesc;
public:

    explicit edit_dialog(QSqlTableModel * _model,QString holder_name = "holder",QString _holder_allias1 = "",QString _holder_allias2 = "",  QString _updated_table1= "",   QString _updated_table2 = "",bool has_lawdesc = true ,QWidget *parent = 0);




signals:

public slots:
    void submit();
    void delete_sel();

    };

#endif // EDIT_DIALOG_H
