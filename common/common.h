#ifndef COMMON_H
#define COMMON_H

#include <netinet/in.h>
#include "ini.h"

typedef struct
{
    //char name[32];
    char ros_ip[32];
    int ros_port;
    int local_port;
}node_info_t;


//UDP
int udp_open(int localport,char *ros_ip,int ros_port);
//int udp_recv(int fd, char *buf,int len,char *ros_ip,int ros_port);
int udp_recv(int fd, char *buf,int len);
int udp_send(int fd, char *buf,int len,char *ros_ip,int ros_port);
int epoll_add(int epfd,int fd);

//解析配置文件

int handler(void* user,const char* section,const char* key,const char* value);
int load_node(const char *name, node_info_t *node);

#endif