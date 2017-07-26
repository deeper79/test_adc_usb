        #include "widget.h"

    Widget::Widget(QWidget *parent)
        : QWidget(parent)
    {

        setGeometry(20,20,1024,600);

        open    =  new QPushButton("Open",this);
        close   =  new QPushButton("Close",this);
        edit    =  new QLabel(this);
        adc_usb =  new adc_usb_class();
        time    =  new QTime();
        time->start();
        str_number = 0;

        open->setGeometry(10,10,100,30);
        close->setGeometry(10,50,100,30);


        edit->setGeometry(130,10,800,700);
        edit->setStyleSheet("background-color :black; color: green");
        edit->setText(QString(data));
        edit->setAlignment(Qt::AlignLeft|Qt::AlignTop);

        connect(open,    SIGNAL(pressed()),  this,  SLOT(openClick()));
        connect(close,   SIGNAL(pressed()),  this,  SLOT(closeClick()));
        connect(adc_usb, SIGNAL(haveData()), this,  SLOT(printData()));

    }
    void Widget::printData(){
        static int count =0;
        bytes_resive++;
        count++;
        if(count>100){
            string = string + getTime()+" Байт принято..... " + QString::number(bytes_resive*4096,10,0) +
                    "  Cкорость передачи ..... " + QString::number((((float)bytes_resive*4096)/(float)IntgetTime())/1024.0/1024.0*1000,10,3) +" Мбайт/c \n" ;
            edit->setText(string);
            count=0;
            if(str_number++>35) str_number =36;

            bytes_resive = 0;
            if(str_number>35){
                for(int i=0;i<string.size();i++){
                    if(string.at(i) == '\n'){
                        string.remove(0,i+1);
                        break;
                    }
                }

            }

        }





    }
    QString Widget::getTime(){

        time->restart();

        return "time "+QString::number(time->hour())
                             + ":" + QString::number(time->minute())
                             + ":" + QString::number(time->second());
    }

    int Widget::IntgetTime(){
        int temp;

        temp =  time->elapsed();

        time->restart();

        return temp;
    }

    void Widget::openClick(){


        if(adc_usb->open(QIODevice::ReadWrite)){

            string.append(getTime() + ".....File open\r\n");

        }else{
            string.append(getTime() + ".....File cannot open\r\n");


        }
        edit->setText(string);


    }

    void Widget::closeClick(){
        time->restart();
        adc_usb->close(QIODevice::ReadWrite);

        string.append("time "+QString::number(time->hour())
                      + ":" + QString::number(time->minute())
                      + ":" + QString::number(time->second())
                      + ".....File close\n");
        edit->setText(string);
    }

    Widget::~Widget()
    {

        delete open;
        delete close;
        delete edit;
        delete adc_usb;

    }
