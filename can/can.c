#include "common.h"
#include "ini.h"
#include "can.h"
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <linux/can.h>
#include <linux/can/raw.h>
#include <net/if.h>
#include <arpa/inet.h>
#include <sys/time.h>




uint8_t rxxbuf[13];

int can_open(can_t *can)
{
    
    struct sockaddr_can can_addr;
    struct ifreq ifr;

    int can_fd = socket(PF_CAN,SOCK_RAW,CAN_RAW);

    if(can_fd < 0)
    {
        perror("socket error");
        return -1;
    }

    //指定CAN设备
    memset(&ifr, 0, sizeof(ifr));
    strcpy(ifr.ifr_name, can->ifname);
    if(ioctl(can_fd, SIOCGIFINDEX, &ifr) < 0)
    {
        perror("ioctl");
        close(can_fd);
        return -1;
    }
    memset(&can_addr, 0, sizeof(can_addr));
    can_addr.can_family  = AF_CAN;
    can_addr.can_ifindex = ifr.ifr_ifindex;
    //将CAN设备与套接字进行绑定
    if(bind(can_fd,(struct sockaddr *)&can_addr,sizeof(can_addr)) < 0)
    {
        perror("bind error");
        close(can_fd);
        return -1;
    }
    return can_fd;
}



int can_read(int fd,struct can_frame *frame)
{
    return read(fd,frame,sizeof(struct can_frame));
}

int can_write(int fd,struct can_frame *frame)
{
    return write(fd,frame,sizeof(struct can_frame));
}

