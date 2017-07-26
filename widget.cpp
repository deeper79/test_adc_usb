#include "widget.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
{

    setGeometry(20,20,1024,800);

    open    =  new QPushButton("Open",this);
    close   =  new QPushButton("Close",this);
    start   =  new QPushButton("Start",this);
    stop    =  new QPushButton("Stop",this);
    adc_usb =  new linux_usb_class();
    time    =  new QTime();
    time->start();
    str_number = 0;

    open->setGeometry(10,10,100,30);
    close->setGeometry(10,50,100,30);
    start->setGeometry(10,90,100,30);
    stop->setGeometry(10,130,100,30);


    //connect(open,    SIGNAL(pressed()),  this,  SLOT(openClick()));
    //connect(close,   SIGNAL(pressed()),  this,  SLOT(closeClick()));
    connect(start,   SIGNAL(pressed()),  this,  SLOT(startClick()));
    connect(stop,    SIGNAL(pressed()),  this,  SLOT(stopClick()));

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
    adc_usb->start();
    qDebug() <<  "Device start";
}

void Widget::stopClick(){
    adc_usb->stop();
    qDebug() <<  "Device stopped\n";
}


Widget::~Widget()
{

    delete open;
    delete close;
    delete start;
    delete stop;
    delete adc_usb;

}
