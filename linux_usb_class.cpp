#include "linux_usb_class.h"

linux_usb_class::linux_usb_class(QObject *parent)
    :QObject(parent)
{
   started = false; //флаг устройство запущено
   buf = (char *)malloc(1023*12);
}
void linux_usb_class::start_read(){
    if(started) return;
    if(ioctl(fd,DEV_CMD_START,&len)<0) {//передача команды начала преобразования
        qDebug() <<"Error ioctl failed";
        return;
    }
   qDebug() <<  "Device start";
    started = true;
}
void linux_usb_class::stop_read(){
    if(!started) return;
    if(ioctl(fd,DEV_CMD_STOP,&len)<0){
        qDebug() <<"Error ioctl failed";
        return;
    }
     started = false;

  qDebug() <<  "Device stopped\n";

}

bool linux_usb_class::open_dev(){ 
    fd = open(dev,O_RDWR);
    if(fd == -1) return false;
    return true;
}
void linux_usb_class::close_dev(){
    close(fd);
}

int linux_usb_class::read_data(){

    struct pollfd ufds[1];
    int timeout = 10000;	/* time out for poll */
    int rc;
    static char count=0;

    ufds[0].fd = fd;
    ufds[0].events = POLLIN;

    for (;;) {
        if ((rc = poll(ufds, 1, timeout)) < 0)   qDebug() <<"Failure in poll\n";
        if (rc > 0) {
            if (ufds[0].revents & POLLIN) {
                rc = read(fd, (buf+1023*count), 1023);
                count++;
                if(count>12){
                    emit haveData(buf);
                    count = 0;
                }
            }else  qDebug() << "poll timed out in " <<timeout <<"milliseconds";
        }
    }

    return rc;
}
int linux_usb_class::write_data(char *data, int len){
    Q_UNUSED(data);
    Q_UNUSED(len);

    return 0;

}

