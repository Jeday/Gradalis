#include "autofillcombobox.h"

autoFillComboBox::autoFillComboBox(QComboBox *parent)
    : QComboBox(parent)
{
    this->setEditable(true);
    setInsertPolicy(QComboBox::NoInsert);

    //connect(this,SIGNAL(editTextChanged(QString)),this,SLOT(addentry(QString)));
    //1000005

}

void autoFillComboBox::keyPressEvent(QKeyEvent *pe){

    if(  pe->key() == 16777220){
        QString test = currentText();
        if (!test.replace(" ","").isEmpty())
           addItem(this->currentText());
    }
    else QComboBox::keyPressEvent(pe);


}

autoFillComboBox::~autoFillComboBox()
{

}

void autoFillComboBox::add_entry(QString s){
     emit new_entry_added(s);
     addItem(s);
 }

void autoFillComboBox::addentry(QString s){
    this->addItem(s);
}

