#include "pwm.h"
#include <stdio.h>
#include <string.h>
#include <unistd.h>

//#define PWM_PATH "/sys/class/pwm/pwmchip%d/pwm0"





int write_file(const char *path,const char *value)
{
    FILE *fp;
    fp=fopen(path,"w");
    if(fp==NULL)
    {
        perror(path);
        return -1;
    }

    fprintf(fp,"%s",value);
    fclose(fp);
    return 0;
}



void get_pwm_path(int chip,const char *name,char *path,int len)
{
    snprintf(path,len,"/sys/class/pwm/pwmchip%d/pwm0/%s",chip,name);
}


int pwm_export(int chip)
{
    char path[128];
    char value[16];
    snprintf(path,sizeof(path),"/sys/class/pwm/pwmchip%d/export",chip);
    /*
     * pwm0
     */
    snprintf(value,sizeof(value),"0");
    return write_file(path,value);
}


int pwm_write_uint(int chip,const char *name,uint32_t value)
{

    char path[128];
    char buf[32];
    get_pwm_path(chip,name,path,sizeof(path));
    snprintf(buf,sizeof(buf),"%u",value);
    return write_file(path,buf);
}


int pwm_set_period(int chip,uint32_t period)
{

    return pwm_write_uint(chip,"period",period);
}


int pwm_set_duty(int chip,uint32_t duty)
{

    return pwm_write_uint(chip,"duty_cycle",duty);
}



int pwm_set_polarity(int chip,pwm_polarity_t polarity)
{
    char path[128];
    get_pwm_path(chip,"polarity",path,sizeof(path));
    if(polarity == PWM_POLARITY_INVERSED)
    {
        return write_file(path,"inversed");
    }
    else if(polarity == PWM_POLARITY_NORMAL)
    {

        return write_file(path,"normal");
    }

}


int pwm_set_enable(int chip,int enable)
{

    return pwm_write_uint(chip,"enable",enable);
}


int pwm_apply(pwm_t *pwm)
{

    /*
     * export pwm0
     */
    pwm_export(pwm->chip);


    /*
     * 等待sysfs节点生成
     */
    usleep(100000);



    /*
     * 修改参数前关闭PWM
     */
    pwm_set_enable(pwm->chip,0);

    pwm_set_polarity(pwm->chip,pwm->polarity);

    pwm_set_period(pwm->chip,pwm->period);

    pwm_set_duty(pwm->chip,pwm->duty);

    pwm_set_enable(pwm->chip,pwm->enable);

    return 0;
}

