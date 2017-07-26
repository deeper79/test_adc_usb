#ifndef WIDGET_H
#define WIDGET_H


#include <QWidget>
#include <QPushButton>
#include <QLineEdit>
#include <QLabel>
#include <linux_usb_class.h>
#include <QTime>
#include <QThread>


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

public slots:

    void openClick(void);
    void closeClick(void);
    void startClick(void);
    void stopClick(void);

};

#endif // WIDGET_H
