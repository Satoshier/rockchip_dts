#ifndef USART_H
#define USART_H

typedef struct
{
    char name[32];
    char dev[64];
    int baudrate;
//    char ip[32];
//    int port;
} uart_t;


int uart_init_cfg(uart_t *uart);

int uart_read(int fd,char *buf,int len);

int uart_write(int fd,char *buf,int len);


#endif