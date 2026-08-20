#include "usart.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <errno.h>
#include <string.h>
#include <signal.h>
#include <termios.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/time.h>


static speed_t get_baudrate(int baudrate)
{
    switch(baudrate)
    {
        case 9600: return B9600;
        case 19200: return B19200;
        case 38400: return B38400;
        case 57600: return B57600;
        case 115200: return B115200;
        default: return B115200;
    }
}

int uart_init_cfg(uart_t *uart)
{

    int fd =open(uart->dev,O_RDWR | O_NOCTTY | O_NDELAY);
    
    if(fd<0)
    {
        fprintf(stderr, "open error: %s: %s\n", uart->dev, strerror(errno));
        return -1;
    }
    //保存终端的配置参数
    struct termios tty;
    cfsetospeed(&tty, get_baudrate(uart->baudrate));
    cfsetispeed(&tty, get_baudrate(uart->baudrate));
    //配置控制模式
    tty.c_cflag |= (CLOCAL | CREAD); // 本地连接, 使能接收

    tty.c_cflag &= ~CSIZE;
    tty.c_cflag |= CS8;  //8位数据为

    tty.c_cflag &= ~PARENB; //无校验
    tty.c_cflag &= ~CSTOPB; //1位停止位
   // tty.c_cflag &= ~CRTSCTS; 

   //待定TODO
    tty.c_lflag = 0;
    tty.c_oflag = 0;
    tty.c_iflag = 0;

    //TODO，根据不同的串口设备设置
    // tty.c_cc[VMIN]  = 0;  // read()至少要收到多少字节才返回
    // tty.c_cc[VTIME] = 1;  //超时时间，单位100ms
    
    //清空输入输出缓冲区
    tcflush(fd,TCIFLUSH);

    if(tcsetattr(fd, TCSANOW, &tty) != 0)
    {
        perror("tcsetattr error");
        return -1;
    }
    return fd;
}


int uart_read(int fd,char *buf,int len)
{
    return read(fd,buf,len);
}

int uart_write(int fd,char *buf,int len)
{
    return write(fd,buf,len);
}

