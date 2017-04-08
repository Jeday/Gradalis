#ifndef CHARTS_H
#define CHARTS_H

#include <QObject>
#include <QtCharts>
#include <QString>
#include <QtCharts>

using namespace QtCharts;

class ChangeColor: public QObject
{
    Q_OBJECT

public:
        ~ChangeColor();
    QBarSet * bset;
    QPieSlice * pslice;
    ChangeColor(QBarSet * b, QPieSlice * p, QObject * parent = 0);

public slots:
    void color_changed(QColor);
};


class MyCharts : public QObject
{
    Q_OBJECT

    QStackedBarSeries * bseries;
    QPieSeries * pseries;

public:
    QChart * stacked_bar_chart;
    QChart * pie_chart;
    QList<ChangeColor*> * list_color;
    QBarCategoryAxis * axisX;
    QValueAxis * axisY;

    MyCharts(QChart * chart1, QChart * chart2, QObject *parent = 0);
    ~MyCharts();

private:
    void add_one_bar(QString name, QList<qreal> value);

    void add_pie();

    friend void set_color_and_label_pie(MyCharts * c);

public:
    void add(QList<QString> * names, QList<QList<qreal>*> * values, const QDate start_time,
                  const QDate finish_time, const int freq, const QString s);
};


#endif // CHARTS_H
