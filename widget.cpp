#include "widget.h"
QT_CHARTS_USE_NAMESPACE
Widget::Widget(QWidget *parent)
    : QWidget(parent)
{

    setGeometry(20,20,1024,800);
    buffer_in = (char *)malloc(1023*12);

    chart = new QChart();
    axisX = new QValueAxis;
    axisY = new QValueAxis;

    open       =  new QPushButton("Open",this);
    close      =  new QPushButton("Close",this);
    start      =  new QPushButton("Start",this);
    stop       =  new QPushButton("Stop",this);
    adc_usb    =  new linux_usb_class();
    time       =  new QTime();
    thread_usb =  new QThread();
    //LineSerias =  new QLineSeries(this);
    chartview  =  new QChartView(this);
    //  LineSerias->setUseOpenGL(true);
    time->start();
    str_number = 0;
    for(int i=0;i<12;i++)
        LineSerias << new QLineSeries;

    for(int i=0;i<512;i++)
        points <<  QPointF(i,10);


    //  points << QPointF(0,0);
    //  points << QPointF(1,20);
    //  points << QPointF(2,-10);


    //LineSerias[0]->insert(0,QPointF(0,10));
    LineSerias[0]->replace(points);


    chart->addAxis(axisX,Qt::AlignBottom);
    chart->addAxis(axisY,Qt::AlignLeft);
    for(int i=0;i<12;i++){

        LineSerias[i]->replace(points);
        chart->addSeries(LineSerias[i]);
        LineSerias[i]->attachAxis(axisX);
        LineSerias[i]->attachAxis(axisY);
    }


    chartview->setChart(chart);





    open->setGeometry(10,10,100,30);
    close->setGeometry(10,50,100,30);
    start->setGeometry(10,90,100,30);
    stop->setGeometry(10,130,100,30);
    chartview->setGeometry(120,10,900,600);


    connect(open,      SIGNAL(pressed())      ,this,    SLOT(openClick()));
    connect(close,     SIGNAL(pressed())      ,this,    SLOT(closeClick()));
    connect(start,     SIGNAL(pressed())      ,this,    SLOT(startClick()));
    connect(stop,      SIGNAL(pressed())      ,this,    SLOT(stopClick()));
    connect(thread_usb,SIGNAL(started())      ,adc_usb, SLOT(read_data()));
    connect(adc_usb,   SIGNAL(haveData(char *)),this,    SLOT(dataRecived(char *)));

    adc_usb->moveToThread(thread_usb);

}
void Widget::dataRecived(char *data){
    uchar max=0,min =0;
    memcpy(buffer_in,data,512*12);

    max = (uchar)*buffer_in;
    min = (uchar)*buffer_in;
    for(int j=0;j<12;j++){
       for(int i=0;i<512;i++){
            points[i] = QPointF(i,(uchar)*(buffer_in+i+j*512));
            if(max<(uchar)*(buffer_in+i+j*512)) max = (uchar)*(buffer_in+i+j*512);
            if(min>(uchar)*(buffer_in+i+j*512)) min = (uchar)*(buffer_in+i+j*512);
        }
        LineSerias[j]->replace(points);
    }

    float minf =(float)min *0.99;
    float maxf =(float)max *1.1;

    chartview->chart()->axisY()->setMax(64);
    chartview->chart()->axisY()->setMin(0);

    //qDebug() << "data recived";
}

void Widget::openClick(){

    if(adc_usb->open_dev()) qDebug() << "File open\n";
    else                    qDebug() << "File cannot open\r\n";
}

void Widget::closeClick(){
    adc_usb->close_dev();
    qDebug() <<  "File close\n";
}

void Widget::startClick(){
    adc_usb->start_read();
    thread_usb->start();

}

void Widget::stopClick(){
    adc_usb->stop_read();
    thread_usb->exit();

}

Widget::~Widget()
{
    delete open;
    delete close;
    delete start;
    delete stop;
    delete adc_usb;
}
