#ifndef AUTOFILLCOMBOBOX_H
#define AUTOFILLCOMBOBOX_H

#include <QWidget>
#include <QComboBox>
#include <QString>
#include <QList>
#include <QKeyEvent>
#include <QDebug>

class autoFillComboBox : public QComboBox
{
    Q_OBJECT
public slots:
    void addentry(QString s);

signals:

    /// signal for model
    void new_entry_added(QString);

public:
    autoFillComboBox(QComboBox *parent = 0);
    ~autoFillComboBox();

    void add_entry(QString s);

protected:
void keyPressEvent(QKeyEvent* pe);

};



#endif // AUTOFILLCOMBOBOX_H
