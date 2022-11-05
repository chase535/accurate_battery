#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void set_value(char *file, char *numb)
{
    FILE *fn;
    if((file != NULL) && (access(file, W_OK) == 0))
    {
        fn = fopen(file, "wt");
        if(fn != NULL)
        {
            fputs(numb, fn);
            fclose(fn);
            fn = NULL;
        }
        else
        {
            printf("无法读取%s文件！\n", file);
            exit(3);
        }
    }
    else
    {
        printf("无法向%s文件写入数据！\n", file);
        exit(4);
    }
}

int main()
{
    FILE *fm, *fp;
    char battery[4], status[20];
    int power[6], full[1];
    *full = 0;
    while(1)
    {
        memset(battery, '\0', sizeof(battery));
        memset(status, '\0', sizeof(status));
        memset(power, '0', sizeof(power));
        fm = fopen("/sys/class/power_supply/bms/capacity_raw", "rt");
        if(fm != NULL)
        {
            fscanf(fm, "%d", power);
            *power += 50;
            if(*power > 9999)
            {
                printf(">99");
                if(! *full)
                {
                    fp = fopen("/sys/class/power_supply/bms/status", "rt");
                    if(fp != NULL)
                    {
                        fscanf(fp, "%s", status);
                        printf("%s", *status);
                        *full = (strcmp(status, "Charging") == 0)?0:1;
                        fclose(fp);
                        fp = NULL;
                    }
                    else
                    {
                        printf("无法读取电流！\n");
                        exit(1);
                    }
                    *battery = (*full)?100:99;
                }
                else
                {
                    printf("100");
                    *battery = 100;
                }
            }
            else
            {
                *full = 0;
                if(*power > 999)
                    snprintf(battery, 3, "%d", *power);
                else if(*power > 99)
                    snprintf(battery, 2, "%d", *power);
                else
                    *battery = 0;
            }
            set_value("/sys/class/power_supply/battery/capacity", battery);
            fclose(fm);
            fm = NULL;
        }
        else
        {
            printf("无法读取电量！\n");
            exit(2);
        }
        sleep(1);
    }
    return 0;
}
