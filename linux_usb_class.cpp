#include "linux_usb_class.h"

linux_usb_class::linux_usb_class(QObject *parent)
    :QObject(parent)
{
    buf.resize(1023);
    started = false;

}
void linux_usb_class::start(){
    if(started) return;
    if(ioctl(fd,DEV_CMD_START,&len)<0) qDebug() <<"Error ioctl failed";

    started = true;
}
void linux_usb_class::stop(){
    if(!started) return;
    if(ioctl(fd,DEV_CMD_STOP,&len)<0)  qDebug() <<"Error ioctl failed";
}

bool linux_usb_class::open_dev(){ 
    fd = open(dev,O_RDWR);
    if(fd == -1) return false;
    return true;
}
void linux_usb_class::close_dev(){
      close(fd);
}

int linux_usb_class::read_data(char *data, int len){
    int retval =0;

    char *buf;
    struct pollfd ufds[1];
    int timeout = 10000;	/* time out for poll */
    int rc;
    int k=0;
    QByteArray array;

    ufds[0].fd = fd;
    ufds[0].events = POLLIN;


    qDebug() <<"buffer size...." << len;

    buf = (char *)malloc(1023);

    for (;;) {

        if ((rc = poll(ufds, 1, timeout)) < 0) {
            qDebug() <<"Failure in poll\n";

        }

        if (rc > 0) {

            if (ufds[0].revents & POLLIN) {
                rc = read(fd, buf, 1023);


            } else {

            }

        } else {
            qDebug() << "poll timed out in " <<timeout <<"milliseconds";
        }
    }



    return retval;

}
int linux_usb_class::write_data(char *data, int len){
    return 0;

}

void linux_usb_class::timerEvent(QTimerEvent *){


}
