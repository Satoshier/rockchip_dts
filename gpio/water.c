#include "water.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define GPIO_BASE_PATH "/sys/class/gpio"

static const int water_gpio[WATER_GPIO_NUM] =
{
    15,     /* GPIO0_B7 */
    19,     /* GPIO0_C3 */
    22,     /* GPIO0_C6 */
    24,     /* GPIO0_D0 */
    25,     /* GPIO0_D1 */
    23      /* GPIO0_C7 */
};


/*
 * 配置GPIO属性
 */
static int gpio_config(int gpio,const char *attr,const char *val)
{
    char file_path[128];

    int fd;
    int len;

    snprintf(file_path,sizeof(file_path),"%s/gpio%d/%s",GPIO_BASE_PATH,gpio,attr);
    fd = open(file_path, O_WRONLY);
    if (fd < 0)
    {
        perror(file_path);
        return -1;
    }

    len = strlen(val);
    if (write(fd, val, len) != len)
    {
        perror("write");
        close(fd);
        return -1;
    }

    close(fd);
    return 0;
}


/*
 * 导出GPIO
 */
static int gpio_export(int gpio)
{
    char gpio_path[128];
    char gpio_num[16];

    int fd;
    int len;

    snprintf(gpio_path,sizeof(gpio_path),"%s/gpio%d",GPIO_BASE_PATH,gpio);

    /*
     * 已经导出
     */
    if (access(gpio_path, F_OK) == 0)
        return 0;

    fd = open(GPIO_BASE_PATH "/export",O_WRONLY);

    if (fd < 0)
    {
        perror("open export");
        return -1;
    }

    snprintf(gpio_num,sizeof(gpio_num),"%d",gpio);
    len = strlen(gpio_num);
    if (write(fd, gpio_num, len) != len)
    {
        if (errno != EBUSY)
        {
            perror("export");
            close(fd);
            return -1;
        }
    }

    close(fd);

    /*
     * 等待sysfs节点生成
     */
    usleep(1000);
    return 0;
}


/*
 * 初始化6个GPIO
 */
int water_gpio_init(water_t *water)
{
    char file_path[128];

    int i;

    for (i = 0; i < WATER_GPIO_NUM; i++)
        water->gpio_fd[i] = -1;

    /*
     * 初始化6个GPIO
     */
    for (i = 0; i < WATER_GPIO_NUM; i++)
    {
        /*
         * export
         */
        if (gpio_export(water_gpio[i]) < 0)
        {
            printf("export gpio%d failed\n",
                   water_gpio[i]);

            return -1;
        }

        /*
         * 设置为输入
         */
        if (gpio_config(water_gpio[i],"direction","in") < 0)
        {
            printf("gpio%d direction failed\n",
                   water_gpio[i]);
            return -1;
        }

        /*
         * 正常极性
         */
        if (gpio_config(water_gpio[i],"active_low","0") < 0)
        {
            printf("gpio%d active_low failed\n",
                   water_gpio[i]);
            return -1;
        }

        /*
         * 不使用edge中断
         */
        if (gpio_config(water_gpio[i],"edge","none") < 0)
        {
            printf("gpio%d edge failed\n",
                   water_gpio[i]);
            return -1;
        }

        /*
         * 打开value
         */
        snprintf(file_path,sizeof(file_path),"%s/gpio%d/value",GPIO_BASE_PATH,water_gpio[i]);
        water->gpio_fd[i] =open(file_path, O_RDONLY);
        if (water->gpio_fd[i] < 0)
        {
            perror(file_path);
            return -1;
        }
    }

    printf("water gpio init success\n");
    return 0;
}


/*
 * 读取一个GPIO
 */
static int water_gpio_read(water_t *water,
                           int index)
{
    char value;

    if (index < 0 ||index >= WATER_GPIO_NUM)
        return -1;

    /*
     * 每次读取前回到文件开头
     */
    lseek(water->gpio_fd[index],0, SEEK_SET);

    if (read(water->gpio_fd[index],&value,1) != 1)
    {
        perror("read gpio");
        return -1;
    }

    if (value == '1')
        water->state[index] = 1;
    else
        water->state[index] = 0;
    return 0;
}


/*
 * 读取全部6个GPIO
 */
int water_gpio_read_all(water_t *water)
{
    for (int i = 0; i < WATER_GPIO_NUM; i++)
    {
        if (water_gpio_read(water, i) < 0)
            return -1;
    }

    return 0;
}


// /*
//  * UDP初始化
//  */
// int water_udp_init(water_t *water,
//                    const char *ip,
//                    int port)
// {
//     water->udp_fd = socket(
//         AF_INET,
//         SOCK_DGRAM,
//         0);

//     if (water->udp_fd < 0)
//     {
//         perror("socket");
//         return -1;
//     }

//     memset(
//         &water->udp_addr,
//         0,
//         sizeof(water->udp_addr));

//     water->udp_addr.sin_family =
//         AF_INET;

//     water->udp_addr.sin_port =
//         htons(port);

//     if (inet_pton(
//             AF_INET,
//             ip,
//             &water->udp_addr.sin_addr) <= 0)
//     {
//         printf("invalid ip: %s\n", ip);

//         close(water->udp_fd);

//         water->udp_fd = -1;

//         return -1;
//     }

//     return 0;
// }


/*
 * UDP发送
 *
 * state[0] = GPIO0_B7
 * state[1] = GPIO0_C3
 * state[2] = GPIO0_C6
 * state[3] = GPIO0_D0
 * state[4] = GPIO0_D1
 * state[5] = GPIO0_C7
 */
// int water_udp_send(water_t *water)
// {
//     int ret;

//     ret = sendto(
//         water->udp_fd,
//         water->state,
//         WATER_GPIO_NUM,
//         0,
//         (struct sockaddr *)&water->udp_addr,
//         sizeof(water->udp_addr));

//     if (ret < 0)
//     {
//         perror("sendto");
//         return -1;
//     }

//     return 0;
// }


/*
 * 关闭
 */
// void water_close(water_t *water)
// {
//     int i;

//     for (i = 0; i < WATER_GPIO_NUM; i++)
//     {
//         if (water->gpio_fd[i] >= 0)
//         {
//             close(water->gpio_fd[i]);
//             water->gpio_fd[i] = -1;
//         }
//     }

//     if (water->udp_fd >= 0)
//     {
//         close(water->udp_fd);
//         water->udp_fd = -1;
//     }
// }