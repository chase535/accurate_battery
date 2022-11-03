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
    char battery[4];
    int power, current, full=0;
    while(1)
    {
        memset(battery, '\0', sizeof(battery));
        fm = fopen("/sys/class/power_supply/bms/capacity_raw", "rt");
        if(fm != NULL)
        {
            fscanf(fm, "%d", &power);
            if(power == 10000)
            {
                if(! full)
                {
                    fp = fopen("/sys/class/power_supply/bms/current_now", "rt");
                    if(fp != NULL)
                    {
                        fscanf(fp, "%d", &current);
                        full = (current)?0:1;
                        fclose(fp);
                        fp = NULL;
                    }
                    else
                    {
                        printf("无法读取电流！\n");
                        exit(1);
                    }
                    (full)?sprintf(battery, "100"):sprintf(battery, "99");
                }
                else sprintf(battery, "100");
            }
            else
            {
                power += 50;
                full = 0;
                if(power > 9999)
                    sprintf(battery, "99");
                else if(power > 999)
                    snprintf(battery, 3, "%d", power);
                else if(power > 99)
                    snprintf(battery, 2, "%d", power);
                else
                    sprintf(battery, "0");
            }
            set_value("/sys/class/power_supply/battery/capacity", battery);
            set_value("/sys/class/power_supply/bms/capacity", battery);
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
