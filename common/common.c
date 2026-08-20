#include "ini.h"
#include "common.h"
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/epoll.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <stdlib.h>

#define NODE_CFG "config/dev_nodes.ini"
static char target_name[32];

node_info_t uart_node={0}; 
node_info_t can_node={0}; 
node_info_t water_node={0}; 

int udp_open(int localport,char *ros_ip,int ros_port)
{
    struct sockaddr_in local_addr;
    int fd =socket(AF_INET, SOCK_DGRAM, 0);

    if(fd < 0) return -1;
    memset(&local_addr,0,sizeof(local_addr));

    local_addr.sin_family = AF_INET;
    local_addr.sin_addr.s_addr =INADDR_ANY;
    local_addr.sin_port = htons(localport);

    if(bind(fd,(struct sockaddr*)&local_addr,sizeof(local_addr)) < 0)
    {
        perror("bind failed");
        close(fd);
        return -1;
    }
    return  fd;
}

// int udp_recv(int fd, char *buf,int len,char *ros_ip,int ros_port)
// {
//     struct sockaddr_in cli_addr;
//     memset(&cli_addr,0,sizeof(cli_addr));
//     cli_addr.sin_family=AF_INET;
//     cli_addr.sin_port=htons(ros_port);
//     cli_addr.sin_addr.s_addr=inet_addr(ros_ip);
//     socklen_t slen =sizeof(cli_addr);
//     return recvfrom(fd,buf,len,0,(struct sockaddr*)&cli_addr,&slen);
// }

int udp_recv(int fd, char *buf,int len)
{
    struct sockaddr_in cli_addr;
    socklen_t addr_len = sizeof(cli_addr);
    return recvfrom(fd,buf,len,0,(struct sockaddr*)&cli_addr,&addr_len);
}

int udp_send(int fd, char *buf,int len,char *ros_ip,int ros_port)
{
    //校验 TODO
    struct sockaddr_in cli_addr;
    memset(&cli_addr,0,sizeof(cli_addr));
    cli_addr.sin_family=AF_INET;
    cli_addr.sin_port=htons(ros_port);
    cli_addr.sin_addr.s_addr=inet_addr(ros_ip);
    // if (inet_pton(AF_INET, ros_ip, &cli_addr.sin_addr) <= 0)
    // {
    //     perror("inet_pton failed");
    //     return -1;
    // }
    return sendto(fd,buf,len,0,(struct sockaddr*)&cli_addr,sizeof(cli_addr));
}


int epoll_add(int epfd,int fd)
{
    struct epoll_event ev;
    memset(&ev,0,sizeof(ev));
    ev.events = EPOLLIN | EPOLLET;
    ev.data.fd = fd;
    return epoll_ctl(epfd,EPOLL_CTL_ADD,fd,&ev);
}

int handler(void* user,const char* section,const char* key,const char* value)
{

    node_info_t *node=user;
   //printf("ttttttttttttttt\n");
    if(strcmp(section,target_name))
    {
        return 1;   
    }
    if(!strcmp(key,"ros_ip"))
    {
        strcpy(node->ros_ip,value);
        printf("node->ros_ip:%s\n",node->ros_ip);
         printf("A\n");
    }
    else if(!strcmp(key,"ros_port"))
    {
        node->ros_port=atoi(value);
        printf("node->ros_port:%d\n",node->ros_port);
          printf("B\n");
    }
    else if(!strcmp(key,"local_port"))
    {
        node->local_port=atoi(value);
        printf("node->local_port:%d\n",node->local_port);
        printf("C\n");
    }

    return 1;
}

int load_node(const char *name, node_info_t *node)
{
    strcpy(target_name,name);
    printf("target_name:%s\n",target_name);   
    return ini_parse(NODE_CFG,handler,node);
}
