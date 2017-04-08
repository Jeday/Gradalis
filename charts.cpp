#include "charts.h"

ChangeColor::ChangeColor(QBarSet * b, QPieSlice * p, QObject * parent):
    bset(b), pslice(p), QObject(parent){
    connect(b, SIGNAL(colorChanged(QColor)), this, SLOT(color_changed(QColor)));
}

void ChangeColor::color_changed(QColor c){
    pslice->setColor(c);
}

ChangeColor::~ChangeColor(){
    this->bset = nullptr;
    this->pslice = nullptr;
}

MyCharts::MyCharts(QChart * chart1, QChart * chart2, QObject *parent)
    : stacked_bar_chart(chart1), pie_chart(chart2), QObject(parent)
{
    bseries = new QStackedBarSeries();
    pseries = new QPieSeries();
    list_color = new QList<ChangeColor*>;
    axisX = new QBarCategoryAxis;
    axisY = new QValueAxis;
}

MyCharts::~MyCharts(){}

void MyCharts::add_one_bar(QString name, QList<qreal> value){
    QBarSet * set = new QBarSet(name);
    for (int i = 0; i < value.size(); ++i)
        set->append(value[i]);
    bseries->append(set);
}

void MyCharts::add_pie(){
    foreach(QBarSet* i, bseries->barSets()){
        QString name = i->label();
        qreal sum = i->sum();
        QPieSlice * new_slice = new QPieSlice(name, sum);
        pseries->append(new_slice);
        list_color->append(new ChangeColor(i, new_slice));
    }
}

void set_color_and_label_pie(MyCharts * c){
    foreach(ChangeColor * i, *(c->list_color)){
       i->pslice->setColor(i->bset->color());
       i->pslice->setLabel(QString::number(i->pslice->percentage() * 100, 'g', 3) + "%");
    }
}

void MyCharts::add(QList<QString> * names, QList<QList<qreal>*> * values, const QDate start_time,
              const QDate finish_time, const int freq, const QString  s){

    stacked_bar_chart->removeAxis(axisX);
    delete axisX;
    stacked_bar_chart->removeAxis(axisY);
    delete axisY;

    qDebug()<<stacked_bar_chart->series().count();
    foreach (QAbstractSeries *s, stacked_bar_chart->series()) {
            delete  s;
        }
    stacked_bar_chart->removeAllSeries();

    pie_chart->removeAllSeries();




    bseries = new QStackedBarSeries();
    pseries = new QPieSeries();
    list_color = new QList<ChangeColor*>;

    for(int i = 0; i < names->size(); ++i)
        add_one_bar(names->at(i), *values->at(i));
    stacked_bar_chart->addSeries(bseries);
    stacked_bar_chart->setAnimationOptions(QChart::SeriesAnimations);

    add_pie();
    pseries->setLabelsVisible();
    pie_chart->addSeries(pseries);
    pie_chart->legend()->hide();
    pie_chart->setAnimationOptions(QChart::SeriesAnimations);

    QStringList * cat = new QStringList;
    if (s == "week")
        for(QDate cur_time = start_time; cur_time <= finish_time; cur_time = cur_time.addDays(7 * freq))
            cat->append(cur_time.toString("dd/MM/yyyy"));
    else
        if (s == "month")
            for(QDate cur_time = start_time; cur_time <= finish_time; cur_time = cur_time.addMonths(freq))
                cat->append(cur_time.toString("dd/MM/yyyy"));
    else
        if (s == "year")
            for(QDate cur_time = start_time; cur_time <= finish_time; cur_time = cur_time.addYears(freq))
                 cat->append(cur_time.toString("dd/MM/yyyy"));
    else
            for(QDate cur_time = start_time; cur_time <= finish_time; cur_time = cur_time.addDays(freq))
                cat->append(cur_time.toString("dd/MM/yyyy"));

    axisX= new QBarCategoryAxis;
    axisX->setTitleText("Date");
    axisX->append(*cat);

    stacked_bar_chart->createDefaultAxes();
    stacked_bar_chart->setAxisX(axisX);
    stacked_bar_chart->setFont(QFont("Times",45));

    axisY = new QValueAxis;
    axisY->setLabelFormat("%i");
    axisY->setTitleText("$");
    axisY->setTickCount(15);
    stacked_bar_chart->setAxisY(axisY);
    bseries->attachAxis(axisY);

    stacked_bar_chart->legend()->setVisible(true);
    stacked_bar_chart->legend()->setAlignment(Qt::AlignRight);

    set_color_and_label_pie(this);
    foreach(ChangeColor * c, *list_color){
            delete c;
        }
    delete list_color;
  }





