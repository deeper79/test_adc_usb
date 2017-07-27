#ifndef WIDGET_H
#define WIDGET_H


#include <QWidget>
#include <QPushButton>
#include <QLineEdit>
#include <QLabel>
#include <linux_usb_class.h>
#include <QTime>
#include <QThread>
#include <QtCharts/QLineSeries>
#include <QtCharts/QChartView>
#include <QtCharts>

QT_CHARTS_USE_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = 0);
    ~Widget();
private:
    QPushButton     *open;
    QPushButton     *close;
    QPushButton     *start;
    QPushButton     *stop;
    const char      *data;
    linux_usb_class *adc_usb;
    QTime           *time;
    QThread         *thread_usb;

    long int bytes_resive;
    int str_number;
    char *buffer_in;

    QChartView  *chartview;
    QValueAxis  *axisX;
    QValueAxis  *axisY;
    QChart      *chart;
    QVector<QLineSeries *> LineSerias;
    QVector<QPointF> points;


public slots:

    void openClick(void);
    void closeClick(void);
    void startClick(void);
    void stopClick(void);
    void dataRecived(char *data);

};

#endif // WIDGET_H
