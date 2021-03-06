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

#define  DEV_CMD_START            0
#define  DEV_CMD_STOP             1

//#define LKMC_IOCTL_MAGIC 0x33
//#define LKMC_IOCTL_START     _IOWR(LKMC_IOCTL_MAGIC, 0, int)
//#define LKMC_IOCTL_STOP      _IOWR(LKMC_IOCTL_MAGIC, 1, int)

class linux_usb_class:public QObject
{
    Q_OBJECT
public:
    linux_usb_class(QObject *parent = 0);
    bool open_dev(void);
    void close_dev(void);
    int write_data(char *data,int len);
    void start_read(void);
    void stop_read(void);
 /*   enum {MMAP_DEV_CMD_GET_BUFSIZE,
          DEV_CMD_START,
          DEV_CMD_STOP
         };*/
private:
    int fd;
    uint len;
    const char *dev = DEFAULT_DEVICE;
    QString string;
    struct pollfd pfd;
    bool started;
    char *buf;

signals:
    void getString(QString *str);

    void haveData(char *data);
public slots:
    int read_data ();
};

#endif // LINUX_USB_CLASS_H
