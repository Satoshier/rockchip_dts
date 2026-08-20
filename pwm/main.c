#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#include "pwm.h"



static void usage(char *name)
{

    printf(
    "\nUsage:\n"
    "%s "
    "--chip num "
    "--period ns "
    "--duty ns "
    "--polarity normal/inversed "
    "--enable 0/1\n\n",

    name);

}


int main(int argc,char **argv)
{

    pwm_t pwm;
    memset(&pwm,0,sizeof(pwm));
    pwm.polarity =PWM_POLARITY_NORMAL;
    for(int i=1;i<argc;i++)
    {

        if(strcmp(argv[i],"--chip")==0)
        {
            pwm.chip =atoi(argv[++i]);
        }
        else if(strcmp(argv[i],"--period")==0)
        {
            pwm.period =strtoul(argv[++i],NULL,10);
        }
        else if(strcmp(argv[i],"--duty")==0)
        {
            pwm.duty =strtoul(argv[++i],NULL,10);
        }
        else if(strcmp(argv[i],"--enable")==0)
        {
            pwm.enable =atoi(argv[++i]);
        }
        else if(strcmp(argv[i],"--polarity")==0)
        {

            i++;
            if(strcmp(argv[i],"inversed")==0)
            {
                pwm.polarity =PWM_POLARITY_INVERSED;
            }
            else
            {
                pwm.polarity =PWM_POLARITY_NORMAL;
            }
        }
        else
        {
            printf("unknown argument:%s\n",argv[i]);
            usage(argv[0]);
            return -1;
        }

    }



    printf("chip:%d\n""period:%u\n""duty:%u\n""polarity:%s\n""enable:%d\n",

    pwm.chip,
    pwm.period,
    pwm.duty,

    pwm.polarity?
    "inversed":
    "normal",

    pwm.enable);
    pwm_apply(&pwm);

    return 0;
}