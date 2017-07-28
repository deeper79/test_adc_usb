#ifndef LINUX_USB_CLASS_H
#define LINUX_USB_CLASS_H

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <poll.h> /* poll */
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <QByteArray>
#include <QDebug>
#include <QObject>

#define DEFAULT_DEVICE "/dev/adc-rls1" // название устройства в /dev/
#define ADC_USB_MAGIC (0x33)           // адресс обмена

#define  DEV_CMD_START         _IO(ADC_USB_MAGIC,0) // команда на начало преобразования
#define  DEV_CMD_STOP          _IO(ADC_USB_MAGIC,1) // команда остановки преобразования
#define  DEV_OFFSET_DATA       _IOR(ADC_USB_MAGIC,2,int) // получение текущего смещения в буффере данных


class linux_usb_class:public QObject
{
    Q_OBJECT
public:
    linux_usb_class(QObject *parent = 0); //коструктор класса
    bool open_dev(void);                  // функция открытия устройчтва для чтения
    void close_dev(void);                 // функция закрытия устровства
    int write_data(char *data,int len);   // фукция записи в устройство
    void start_read(void) ;               // фукция включения начала преобразования
    void stop_read(void) ;                // фукция выключения начала преобразования

private:
    int                fd;                   // указатель на устройство
    uint               offset;               // смещение данных в буффере
    const char        *dev = DEFAULT_DEVICE; // название устройства
    struct pollfd      pfd;                  // структура для управления poll
    bool               started;              // флаг работы преобразования
    char              *buf;                  // приемный буффер

signals:
    void haveData(char* data); // сигнал получения данных
public slots:
    int read_data ();          // неблокируемая функция чтения данных
};

#endif // LINUX_USB_CLASS_H
