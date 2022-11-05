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
            printf("无法读取%s文件，程序强制退出！\n", *file);
            exit(3);
        }
    }
    else
    {
        printf("无法向%s文件写入数据，程序强制退出！\n", *file);
        exit(4);
    }
}

int main(char *power_file)
{
    FILE *fm, *fp;
    char battery[4],power_file[100];
    int power[5], current[20], full[1]={0};
    while(1)
    {
        memset(battery, '\0', sizeof(battery));
        if(*power_file == "/sys/class/power_supply/bms/real_capacity")
        {
            fm = fopen(power_file, "rt");
            if(fm != NULL)
            {
                fscanf(fm, "%d", power);
                snprintf(battery, 4, "%d", *power);
                fclose(fm);
                fm = NULL;
            }
            else
            {
                printf("无法读取%s文件，程序强制退出！\n", *power_file);
                exit(2);
            }
        }
        else if(*power_file == "/sys/class/power_supply/bms/capacity_raw")
        {
            fm = fopen(power_file, "rt");
            if(fm != NULL)
            {
                fscanf(fm, "%d", power);
                *power += 50;
                if(*power > 9999)
                {
                    if(! *full)
                    {
                        fp = fopen("/sys/class/power_supply/bms/current_now", "rt");
                        if(fp != NULL)
                        {
                            fscanf(fp, "%d", current);
                            *full = (*current == 0)?1:0;
                            fclose(fp);
                            fp = NULL;
                        }
                        else
                        {
                            printf("无法读取/sys/class/power_supply/bms/current_now文件，程序强制退出！\n");
                            exit(1);
                        }
                        (*full)?snprintf(battery, 4, "100"):snprintf(battery, 3, "99");
                    }
                    else
                    {
                        snprintf(battery, 4, "100");
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
                        snprintf(battery, 2, "0");
                }
                set_value("/sys/class/power_supply/battery/capacity", battery);
                fclose(fm);
                fm = NULL;
            }
            else
            {
                printf("无法读取%s文件，程序强制退出！\n", *power_file);
                exit(2);
            }
        }
        else
        {
            printf("不支持的真实电量文件路径：%s，程序强制退出！\n", *power_file);
            exit(6);
        }
        sleep(1);
    }
    return 0;
}
