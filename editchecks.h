#ifndef EDITCHECKS_H
#define EDITCHECKS_H

#include <QObject>
#include <QWidget>
#include <QtSql>
#include <QLayout>
#include <QtWidgets>
#include <QGroupBox>
#include <QCalendarWidget>
#include <QDebug>
#include <QSqlRelationalTableModel>
#include "main_head.h"
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>


class editChecks : public QWidget
{
    Q_OBJECT
public:
    explicit editChecks(QWidget *parent = 0);

    QSqlRelationalTableModel*model; // sql table model for checks list
    QTableView *view;
    QSqlRelationalTableModel * item_model; // sql model for item list
    QTableView * item_view;


        QSqlTableModel * sender_model;
        QSqlTableModel * rec_model;

        QComboBox * senders;
        QComboBox * reciv;
        QComboBox * projects;

          QLabel * send_label;
            QLabel * recv_label;
            QLabel * proj_label;



        bool in_out_checks; // display cheks in


            QLineEdit * check_name ;
            QLineEdit * check_desc ;
            QDateEdit  * check_rec_date;
            QDateEdit  *  check_pay_date;
            QDateEdit  *  check_payed_date;
            QRadioButton * in_check;
            QPushButton * add_check;


            QComboBox * choose_manuf;
            QSqlRelationalTableModel * item_list_model;
            QComboBox *  choose_item;
            QSpinBox * item_qunt;
            QDoubleSpinBox  * item_price;
            QPushButton * new_item;
             QNetworkAccessManager * manager;




signals:

public slots:
    void change_view();
    void change_item_model(QModelIndex);
    void sumbit_check();
    void del_selected();
    void in_out_changed_new_check(bool);
    void add_item();
    void change_item_filer(int);
    void del_selected_item();
    void manuf_combo_box_clicked();
    void validate_calendars();
    void enable_back();
private:


};

#endif // EDITCHECKS_H
