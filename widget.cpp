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
        points <<  QPointF(i,0);


    //  points << QPointF(0,0);
    //  points << QPointF(1,20);
    //  points << QPointF(2,-10);


    //LineSerias[0]->insert(0,QPointF(0,10));



    chart->addAxis(axisX,Qt::AlignBottom);
    chart->addAxis(axisY,Qt::AlignLeft);
    for(int i=0;i<12;i++){
        LineSerias[i]->replace(points);
        chart->addSeries(LineSerias[i]);

        LineSerias[i]->attachAxis(axisX);
        LineSerias[i]->attachAxis(axisY);

    }
    axisX->setTitleText("points");
    axisY->setTitleText("mV");
    axisY->setTickCount(5);
    axisY->setMinorTickCount(4);

    axisX->setTickCount(17);
    axisX->setMinorTickCount(1);





    chartview->setChart(chart);

    chartview->chart()->axisY()->setMax(128);
    chartview->chart()->axisY()->setMin(-128);




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
    char max=0,min =0;
    memcpy(buffer_in,data,512*12);

    max = *buffer_in;
    min = *buffer_in;

/*    for(int j=0;j<12;j++){
       for(int i=0;i<512;i++){
       *(buffer_in+i+j*512) = *(buffer_in+i+j*512) - 0x80;

       }
    }*/

 // int  j=0;
    for(int j=0;j<12;j++){
       for(int i=0;i<512;i++){

            qreal temp = *(buffer_in+i+j*512);

    //        temp = 2.0*temp/256.0*1000.0;
                     points[i] = QPointF(i,temp );

       //     if(max<*(buffer_in+i+j*512)) max = *(buffer_in+i+j*512);
         //   if(min>*(buffer_in+i+j*512)) min = *(buffer_in+i+j*512);
        }
       LineSerias[j]->replace(points);

    }





   // qDebug() << "data recived";
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
