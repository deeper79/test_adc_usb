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
#define DEFAULT_DEVICE "/dev/adc-rls1"
#define MMAP_DEV_CMD_GET_BUFSIZE 1

class linux_usb_class:public QObject
{
    Q_OBJECT
public:
    linux_usb_class(QObject *parent = 0);
    bool open_dev(void);
    void close_dev(void);
    int write_data(char *data,int len);
    int read_data (char *data,int len);
private:
    int fd;
    uint len;
    const char *dev = DEFAULT_DEVICE;
    QString string;
     char *ptr;
     struct pollfd pfd;
     QByteArray buf;
      void timerEvent(QTimerEvent *);
      char buffer[1024];

      char *mmap_buf; //adress mmap
signals:
    void getString(QString *str);
};

#endif // LINUX_USB_CLASS_H