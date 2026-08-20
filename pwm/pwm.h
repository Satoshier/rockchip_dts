#ifndef PWM_PWM_H
#define PWM_PWM_H

#include <stdint.h>

// #define PWM_CHIP "/sys/class/pwm/pwmchip0"
// #define PWM_CHIP "/sys/class/pwm/pwmchip1"
// #define PWM_CHIP "/sys/class/pwm/pwmchip2"
// #define PWM_CHIP "/sys/class/pwm/pwmchip3"
// #define PWM_CHIP "/sys/class/pwm/pwmchip4"
// #define PWM_CHIP "/sys/class/pwm/pwmchip5"
// #define PWM_CHIP "/sys/class/pwm/pwmchip6"
// #define PWM_CHIP "/sys/class/pwm/pwmchip7"

typedef enum
{
    PWM_POLARITY_NORMAL = 0,
    PWM_POLARITY_INVERSED
} pwm_polarity_t;

typedef struct
{
    int chip;
    uint32_t period;
    uint32_t duty;
    pwm_polarity_t polarity;
    int enable;
} pwm_t;


int pwm_export(int chip);

int write_file(const char *path,const char *value);

void get_pwm_path(int chip,const char *name,char *path,int len);

int pwm_export(int chip);

int pwm_write_uint(int chip,const char *name,uint32_t value);

int pwm_set_period(int chip,uint32_t period);

int pwm_set_duty(int chip,uint32_t duty);

int pwm_set_polarity(int chip,pwm_polarity_t polarity);

int pwm_set_enable(int chip,int enable);


int pwm_apply(pwm_t *pwm);



#endif 
