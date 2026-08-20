#include "common.h"
#include "ini.h"
#include "water.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

extern node_info_t water_node;


int main(int argc, char **argv)
{
    //char udp_buf[512];
    water_t water = {0};

    memset(&water, 0, sizeof(water));

    for(int i=1;i<argc;i++)
    {
        if(!strcmp(argv[i],"--name"))
        {
            strcpy(water.name,argv[++i]);
        }         
    }

    /*
     * 加载配置
     */
    if (load_node(water.name,&water_node) < 0)
    {
        printf("load config failed\n");
        return -1;
    }


    /*
     * GPIO初始化
     */
    if (water_gpio_init(&water) < 0)
    {
        printf("water gpio init failed\n");
        return -1;
    }
    
    //UDP
    int udp_fd = udp_open(water_node.local_port,water_node.ros_ip,water_node.ros_port);
    if(udp_fd<0)
    {
        printf("udp_open failed\n");
        return -1;
    }
   
    printf("water_node start\n");
    
    while (1)
    {
        /*
         * 读取6个GPIO
         */
        if (water_gpio_read_all(&water) < 0)
        {
            printf("read gpio failed\n");
        }
        else
        {
           udp_send(udp_fd,water.state,WATER_GPIO_NUM,water_node.ros_ip,water_node.ros_port); 
        }
        usleep(100000);  //100ms
    }
    return 0;
}