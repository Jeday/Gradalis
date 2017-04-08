#ifndef GRAPH_AREA_H
#define GRAPH_AREA_H


#include <main_head.h>
#include <charts.h>
#include <QWidget>
#include <mainwindow.h>
#include <QList>
#include <QNetworkAccessManager>

//QSqlTableModel * projects_model;


class graph_area : public QWidget
    {
    Q_OBJECT
public:
    explicit graph_area(filter_state * _filter_status,QWidget *parent = 0);


    filter_state * filter_status;
    QDateEdit * start_date;
    QDateEdit * end_date;
    QList<QList<qreal> * > * bchart_values_list;
    QComboBox * bchart_sortby;
    QComboBox * bchart_datetype;
    QSpinBox  * bchart_freq;
    MyCharts * bchart;
    QNetworkAccessManager * manager;
    QChart * chart1;
    QChart * chart2;
    QChartView * chartview1;
    QChartView * chartview2;
private:
    void sort_by_projects();
    void sort_by_clients();
    void sort_by_provider();
    void sort_by_company();
    QDate nextDate(QDate d);

signals:



public slots:
    void enable_back();
    void generate_bchart_data();
    void correct_date();
    void drawgraphs(bool);

private slots:

    };
#endif // GRAPH_AREA_H
