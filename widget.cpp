#include "widget.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
{

    setGeometry(20,20,1024,800);
    buffer_in = (char *)malloc(1023*12);

    open       =  new QPushButton("Open",this);
    close      =  new QPushButton("Close",this);
    start      =  new QPushButton("Start",this);
    stop       =  new QPushButton("Stop",this);
    adc_usb    =  new linux_usb_class();
    time       =  new QTime();
    thread_usb =  new QThread();
    time->start();
    str_number = 0;



    open->setGeometry(10,10,100,30);
    close->setGeometry(10,50,100,30);
    start->setGeometry(10,90,100,30);
    stop->setGeometry(10,130,100,30);


    connect(open,      SIGNAL(pressed())      ,this,    SLOT(openClick()));
    connect(close,     SIGNAL(pressed())      ,this,    SLOT(closeClick()));
    connect(start,     SIGNAL(pressed())      ,this,    SLOT(startClick()));
    connect(stop,      SIGNAL(pressed())      ,this,    SLOT(stopClick()));
    connect(thread_usb,SIGNAL(started())      ,adc_usb, SLOT(read_data()));
    connect(adc_usb,   SIGNAL(haveData(char *)),this,    SLOT(dataRecived(char *)));

    adc_usb->moveToThread(thread_usb);

}
void Widget::dataRecived(char *data){
    memcpy(buffer_in,data,1023*12);
    qDebug() << "data recived";
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
