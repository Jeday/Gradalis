#include "editclients.h"

editClients::editClients(QWidget *parent) : QWidget(parent)
{
            QVBoxLayout * lay = new QVBoxLayout();
            this->setLayout(lay);
            QSqlTableModel* model = new QSqlTableModel(nullptr, QSqlDatabase::database());
            model->setTable("grad.clients");
            model->setEditStrategy(QSqlTableModel::OnFieldChange);
            model->select();
            QTableView *view = new QTableView();
            view->setModel(model);
            lay->addWidget(view);

}
