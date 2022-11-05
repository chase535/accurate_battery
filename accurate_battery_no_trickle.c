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
            printf("无法读取%s文件，程序强制退出！\n", file);
            exit(3);
        }
    }
    else
    {
        printf("无法向%s文件写入数据，程序强制退出！\n", file);
        exit(4);
    }
}

int main(int argc, char *argv[])
{
    FILE *fm;
    char battery[4];
    int power[5];
    while(1)
    {
        memset(battery, '\0', sizeof(battery));
        if(strcmp(argv[1], "/sys/class/power_supply/bms/real_capacity") == 0)
        {
            fm = fopen(argv[1], "rt");
            if(fm != NULL)
            {
                fscanf(fm, "%d", power);
                snprintf(battery, 4, "%d", *power);
                fclose(fm);
                fm = NULL;
            }
            else
            {
                printf("无法读取%s文件，程序强制退出！\n", argv[1]);
                exit(2);
            }
        }
        else if(strcmp(argv[1], "/sys/class/power_supply/bms/capacity_raw") == 0)
        {
            fm = fopen(argv[1], "rt");
            if(fm != NULL)
            {
                fscanf(fm, "%d", power);
                *power += 50;
                if(*power > 9999)
                    snprintf(battery, 4, "100");
                else if(*power > 999)
                    snprintf(battery, 3, "%d", *power);
                else if(*power > 99)
                    snprintf(battery, 2, "%d", *power);
                else
                    snprintf(battery, 2, "0");
                set_value("/sys/class/power_supply/battery/capacity", battery);
                fclose(fm);
                fm = NULL;
            }
            else
            {
                printf("无法读取%s文件，程序强制退出！\n", argv[1]);
                exit(2);
            }
        }
        else
        {
            printf("不支持的真实电量文件路径：%s，程序强制退出！\n", argv[1]);
            exit(6);
        }
        sleep(1);
    }
    return 0;
}
