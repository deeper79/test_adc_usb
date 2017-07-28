#include "linux_usb_class.h"

linux_usb_class::linux_usb_class(QObject *parent)
    :QObject(parent)
{
    started = false;                // флаг устройство запущено
    buf = (char *)malloc(1023*12);  // выделение памяти под буффер приема
}

void linux_usb_class::start_read(){
    if(started) return;                     // если уже работает выход
    if(ioctl(fd,DEV_CMD_START,&offset)<0) { // передача команды начала преобразования
        qDebug() <<"Error ioctl failed";    // ошибка передачи команды на преобразования
        return;
    }
    qDebug() <<  "Device start";            //
    started = true;                         // установка флага вкючения преобразования
}

void linux_usb_class::stop_read(){
    if(!started) return;                    // если не работает выход
    if(ioctl(fd,DEV_CMD_STOP,&offset)<0){   // передача команды остановки преобразования
        qDebug() <<"Error ioctl failed";    // ошибка передача команды остановки преобразования
        return;
    }
    started = false;                        // установка флага вкючения преобразования

    qDebug() <<  "Device stopped\n";        //
}

bool linux_usb_class::open_dev(){ 
    fd = open(dev,O_RDWR);           // открытие устройства
    if(fd == -1) return false;       // ошибка открытия
    return true;                     // файл открыт
}
void linux_usb_class::close_dev(){
    close(fd);                      // закрытие устройства
}


int linux_usb_class::read_data(){

    struct pollfd ufds[1];  // структура управления pol()
    int timeout = 10000;	// таймаут приема данных по usb
    int rc;                 // количество переданных байт
    static char count=0;    // кол-во переданных пакетов

//---------заполнение структуры poll()------------------------------------------

    ufds[0].fd = fd;         //запись указателя на устройсво
    ufds[0].events = POLLIN; //запись события наличия принятых данных

//------------------------------------------------------------------------------

//--------------------бесконечных цикл чтения данных----------------------------
    while(1) {
        if ((rc = poll(ufds, 1, timeout)) < 0)        // запуск в драйвере фукции poll()
            qDebug() <<"Failure in poll\n";           // ошибка при иницилизации фукции poll()
        if (rc > 0) {                                 // ошибок нет
            if (ufds[0].revents & POLLIN) {           // есть данных для передачи
                ioctl(fd,DEV_OFFSET_DATA,&offset);    // получения смещения на полученные данные
                rc = read(fd, (buf+512*count), 512);  // чтение данных
                 count++;                             // увеличиваем кол-во принятых пакетов
                if(count>12){                         // принято 12 пакетов
                    emit haveData(buf);               // взод сигнала получения данных
                    count = 0;                        // обнуление принятых пакетов
                }
            }else  qDebug() << "poll timed out in " <<timeout <<"milliseconds"; // таймаут ожидания данных
        }
    }

    return rc; // возврат кол-ва принятых данных
}
int linux_usb_class::write_data(char *data, int len){
    Q_UNUSED(data);
    Q_UNUSED(len);

    return 0;

}

