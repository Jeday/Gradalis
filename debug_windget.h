#ifndef DEBUG_WINDGET_H
#define DEBUG_WINDGET_H

#include "main_head.h"
#include <QWidget>
#include <QtWidgets>

class debug_windget : public QWidget
    {
    Q_OBJECT
public:
    explicit debug_windget(QWidget *parent = 0);

    QLineEdit * host_name;
    QLineEdit * db_name;
    QLineEdit * user_name;
    QLineEdit * password;
    QLineEdit * port;
    QLabel * status;
signals:

public slots:
    void connect_to_database();

    void init_bd();
};
#endif // DEBUG_WINDGET_H
