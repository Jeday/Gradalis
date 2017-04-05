#ifndef EDIT_ITEMS_H
#define EDIT_ITEMS_H

#include <QWidget>
#include "main_head.h"

class edit_items : public QWidget
    {
    Q_OBJECT
public:
    explicit edit_items(QWidget *parent = 0);

private:
    QComboBox * choose_manufacturers_model;
    QSqlRelationalTableModel * item_model;
    QTableView * item_view;
    QLineEdit * name;



signals:

public slots:
    void submit_item();
    void del_selected();
    };

#endif // EDIT_ITEMS_H
