#ifndef WIDGET_H
#define WIDGET_H


#include <QWidget>
#include <QPushButton>
#include <QLineEdit>
#include <QLabel>
#include <linux_usb_class.h>
#include <QTime>


class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = 0);
    ~Widget();
private:
    QPushButton *open;
    QPushButton *close;
    QLabel      *edit;
    const char  *data;
    linux_usb_class *adc_usb;
    QTime *time;
    QString string;
   inline QString  getTime(void);
   inline int      IntgetTime(void);
   long int bytes_resive;
   int str_number;

public slots:

    void openClick(void);
    void closeClick(void);
    void printData();


};

#endif // WIDGET_H
