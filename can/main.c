#include "common.h"
#include "can.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/epoll.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#include <linux/can/raw.h>

//#define Local_Port 9000
#define RecvCount_UDP 13
extern node_info_t can_node;
//extern node_info_t can_node;

int main(int argc,char **argv)
{
    //char name[32]={0};

    char udp_buf[512];
    char can_buf[512];
    struct can_frame frame;
    can_t can={0};
    struct epoll_event events[2];
    
    for(int i=1;i<argc;i++)
    {
        if(!strcmp(argv[i],"--name"))
        {
            strcpy(can.name,argv[++i]);
            printf("can.name=%s\n",can.name);
        }    

        else if(!strcmp(argv[i],"--ifname"))
        {
            strcpy(can.ifname,argv[++i]);
            printf("can.ifname=%s\n",can.ifname);
        }
    }

    // if(!strlen(can.name) || !strlen(can.ifname))
    // {
    //     printf("can_node " "--name xxx " "--ifname xxx\n");
    //     return -1;
    // }

    if(load_node(can.name,&can_node)<0)
    {
        printf("ini load failed\n");
        return -1;
    }

    int can_fd= can_open(&can);
    if(can_fd<0)
    {
        printf("can_open failed\n");
        return -1;
    }
    printf("can_fd=%d\n",can_fd);

    int udp_fd = udp_open(can_node.local_port,can_node.ros_ip,can_node.ros_port);
    if(udp_fd<0)
    {
        printf("udp_open failed\n");
        return -1;
    }
    printf("udp_fd=%d\n",udp_fd);

    int epfd = epoll_create1(0);
    int a = epoll_add(epfd,can_fd);
    printf("epoll_add can_fd=%d\n",a);
    int b = epoll_add(epfd,udp_fd);
    printf("epoll_add udp_fd=%d\n",b);

    while(1)
    {
        int n = epoll_wait(epfd,events,2,-1);  //500
        printf("n=%d\n",n);
        for(int i=0;i<n;i++)
        {
            if(events[i].data.fd == can_fd)
            {   
                printf("can fdddddddddddddddddddddddddddd \n"); 
                uint8_t txbuf[13];
                int len =can_read(can_fd,&frame);
                printf("len=%d\n",len);
                if(len>0)
                {   
                    uint8_t ctrl = 0;
                    //扩展
                    if(frame.can_id & CAN_EFF_FLAG)
                    {
                        //txbuf[0] = frame.can_dlc;
                        ctrl |= 0x80; 
                        txbuf[1] = ((frame.can_id & CAN_EFF_MASK) >> 24)&0xff;
                        txbuf[2] = ((frame.can_id & CAN_EFF_MASK) >> 16)&0xff;
                        txbuf[3] = ((frame.can_id & CAN_EFF_MASK) >> 8)&0xff;
                        txbuf[4] = (frame.can_id & CAN_EFF_MASK) & 0xff;
                        memcpy(&txbuf[5],frame.data,8);
                    }
                    //标准
                    else if(frame.can_id & CAN_SFF_MASK)
                    {
                       //ID校验 TODO
                       //txbuf[0] = frame.can_dlc;
                       //ctrl |= 0x40;        
                       txbuf[1] = ((frame.can_id & CAN_SFF_MASK) >> 24)&0xff;
                       txbuf[2] = ((frame.can_id & CAN_SFF_MASK) >> 16)&0xff;
                       txbuf[3] = ((frame.can_id & CAN_SFF_MASK) >> 8)&0xff;
                       txbuf[4] = (frame.can_id & CAN_SFF_MASK) & 0xff;
                       memcpy(&txbuf[5],frame.data,8);
                    }
                    ctrl |= frame.can_dlc & 0x0F;  // bit3~0：DLC
                    txbuf[0] = ctrl;
                }
                udp_send(udp_fd,(char*)txbuf,RecvCount_UDP,can_node.ros_ip,can_node.ros_port);
                //memset(&frame,0,sizeof(frame));
            }
            else if(events[i].data.fd==udp_fd)
            {
                printf("udp fdddddddddddddddddddddddddddd \n"); 
                int len =udp_recv(udp_fd,udp_buf,sizeof(udp_buf));
                if(len==RecvCount_UDP)
                {
                    uint32_t udp_id = ((uint32_t)udp_buf[1]<<24)|((uint32_t)udp_buf[2]<<16)|((uint32_t)udp_buf[3]<<8)|(uint32_t)udp_buf[4];
                    memset(&frame,0,sizeof(frame));
                    frame.can_dlc = udp_buf[0] & 0x0F;
           
                    if(udp_buf[0] & 0x80)
                    frame.can_id |= (udp_id & CAN_EFF_MASK) | CAN_EFF_FLAG;
                    else
                    frame.can_id |= udp_id & CAN_SFF_MASK;  
                    //数据帧才复制DATA 
                    if(!(frame.can_id & CAN_RTR_FLAG))
                    {
                        memcpy(frame.data,&udp_buf[5],8);
                        can_write(can_fd,&frame);
                    }
                }
                else
                {
                    continue;
                }
            }
        }
    }
    return 0;
}

