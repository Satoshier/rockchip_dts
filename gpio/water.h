#ifndef WATER_H
#define WATER_H

#include <stdint.h>
#include <netinet/in.h>

#define WATER_GPIO_NUM 6

typedef struct
{
    char name[32];
    int gpio_fd[WATER_GPIO_NUM];
    uint8_t state[WATER_GPIO_NUM];

    //int udp_fd;
    //struct sockaddr_in udp_addr;
} water_t;


int water_gpio_init(water_t *water);

int water_gpio_read_all(water_t *water);

int water_udp_init(water_t *water,const char *ip,int port);

int water_udp_send(water_t *water);

void water_close(water_t *water);

#endif