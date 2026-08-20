#include "common.h"
#include "ini.h"
#include "usart.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/epoll.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>


//#define Local_Port 9000

extern node_info_t uart_node;
//extern node_info_t can_node;



int main(int argc,char **argv)
{
    //char name[32]={0};
    
    char udp_buf[512];
    char uart_buf[512];

    uart_t uart={0};
    struct epoll_event events[2];
    
    for(int i=1;i<argc;i++)
    {
        if(!strcmp(argv[i],"--name"))
        {
            strcpy(uart.name,argv[++i]);
        }
            
        else if(!strcmp(argv[i],"--dev"))
        {
            strcpy(uart.dev,argv[++i]); 
        }
        else if(!strcmp(argv[i],"--baud"))
        {
            uart.baudrate=atoi(argv[++i]);
        }           
    }

   /*
         if(!strlen(uart.name) || !strlen(uart.dev))
        {
        printf("uart_node " "--name xxx " "--dev xxx " "--baud xxx\n");
        return -1;
        }
   */ 
   

    if(load_node(uart.name,&uart_node)<0)
    {
        printf("ini load failed\n");
        return -1;
    }

    int uart_fd= uart_init_cfg(&uart);
    if(uart_fd<0)
    {
        printf("uart_init_cfg failed\n");
        return -1;
    }
    printf("uart_fd=%d\n",uart_fd);
    
    int udp_fd = udp_open(uart_node.local_port,uart_node.ros_ip,uart_node.ros_port);
    if(udp_fd<0)
    {
        printf("udp_open failed\n");
        return -1;
    }
    printf("udp_fd=%d\n",udp_fd);
   
    int epfd = epoll_create1(0);
    int a = epoll_add(epfd,uart_fd);
    printf("a=%d\n",a);
    int b = epoll_add(epfd,udp_fd);
    printf("b=%d\n",b);
    
    printf("start\n");
    //char buf[] = "hello\r\n";
    while(1)
    {
        int n = epoll_wait(epfd,events,2,-1);  //500
        printf("n=%d\n",n);
        for(int i=0;i<n;i++)
        {
            if(events[i].data.fd == uart_fd)
            {
                printf("uart fdddddddddddddddddddddddddddd \n");
                int len =uart_read(uart_fd,uart_buf,sizeof(uart_buf));
                if(len>0)
                udp_send(udp_fd,uart_buf,len,uart_node.ros_ip,uart_node.ros_port);
            }
            else if(events[i].data.fd==udp_fd)
            {
                printf("udp fdddddddddddddddddddddddddddd \n");
                int len =udp_recv(udp_fd,udp_buf,sizeof(udp_buf));
                if(len>0)
                uart_write(uart_fd,udp_buf,len);
            }
        }
    }
    return 0;
}

