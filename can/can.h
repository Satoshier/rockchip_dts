#ifndef BSP_CAN_H
#define BSP_CAN_H

//#include "main.h"
//#include "cfg.h"
#include <stdint.h>
#include <linux/can.h>
//#define CAN_COUNT 4

typedef struct
{
    //int fd;
    char name[32];  //节点名称
    //const char *ifname;  //CAN名称
    char ifname[32];  //CAN名称
} can_t;


int can_open(can_t *can);
//void can_recv(int fd,can_t *can);
//void can_init_all(void);
//int can_send(can_t *can,uint32_t can_id,const uint8_t *data,uint8_t dlc);
int can_read(int fd,struct can_frame *frame);
int can_write(int fd,struct can_frame *frame);


#endif // BSP_CAN_H
