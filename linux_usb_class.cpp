#include "linux_usb_class.h"

linux_usb_class::linux_usb_class(QObject *parent)
    :QObject(parent)
{
     buf.resize(1023);

}

bool linux_usb_class::open_dev(){

    char *buf;
    struct pollfd ufds[1];
    int timeout = 10000;	/* time out for poll */
    int rc;
    int k=0;
    QByteArray array;
    static int p = 0;
    array.resize(1023*12*10);

    fd = open(dev,O_RDWR);
    if(fd == -1) return false;

    ufds[0].fd = fd;
    ufds[0].events = POLLIN;

    if(ioctl(fd,MMAP_DEV_CMD_GET_BUFSIZE,&len)<0){
        qDebug() <<"Error ioctl failed";

    }
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



    return true;

}
void linux_usb_class::close_dev(){
    if(munmap(mmap_buf,len)<0){
        qDebug() << "error";
    }
    close(fd);
}

int linux_usb_class::read_data(char *data, int len){
    int retval =0;



    return retval;

}
int linux_usb_class::write_data(char *data, int len){
    return 0;

}

void linux_usb_class::timerEvent(QTimerEvent *){


}
