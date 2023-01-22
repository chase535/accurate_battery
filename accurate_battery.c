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

void line_feed(char *line)
{
    char *p;
    if((p = strchr(line, '\n')) != NULL)
    {
        *p = '\0';
    }
}

int main(int argc, char *argv[])
{
    FILE *fm, *fp, *fq;
    char battery[10], current_char[30], charge_status[25];
    int power, current, full=0;
    if(argc < 2)
    {
        printf("请传入真实电量文件路径！\n");
        exit(7);
    }
    else if(argc > 2)
    {
        printf("请传入真实电量文件路径，勿传入多余参数！\n");
        exit(20);
    }
    if(strcmp(argv[1], "/sys/class/power_supply/bms/real_capacity") == 0)
    {
        while(1)
        {
            fm = fopen(argv[1], "rt");
            if(fm != NULL)
            {
                fgets(battery, 4, fm);
                fclose(fm);
                fm = NULL;
            }
            else
            {
                printf("无法读取%s文件，程序强制退出！\n", argv[1]);
                exit(2);
            }
            fq = fopen("/sys/class/power_supply/battery/status", "rt");
            if(fq != NULL)
            {
                fgets(charge_status, 20, fq);
                fclose(fq);
                fq = NULL;
            }
            else
            {
                printf("无法读取/sys/class/power_supply/battery/status文件，程序强制退出！\n");
                exit(10);
            }
            line_feed(charge_status);
            power = atoi(battery);
            if(power == 100)
            {
                if(strcmp(charge_status, "Charging") == 0)
                {
                    if(!full)
                    {
                        fp = fopen("/sys/class/power_supply/bms/current_now", "rt");
                        if(fp != NULL)
                        {
                            fgets(current_char, 20, fp);
                            fclose(fp);
                            fp = NULL;
                        }
                        else
                        {
                            printf("无法读取/sys/class/power_supply/bms/current_now文件，程序强制退出！\n");
                            exit(30);
                        }
                        current = atoi(current_char);
                        full=(current < 0)?0:1;
                        (full)?:snprintf(battery, 3, "99");
                    }
                }
                else
                {
                    (full)?:snprintf(battery, 3, "99");
                }
            }
            else
            {
                full = 0;
            }
            set_value("/sys/class/power_supply/battery/capacity", battery);
            sleep(5);
        }
    }
    else if(strcmp(argv[1], "/sys/class/power_supply/bms/capacity_raw") == 0)
    {
        while(1)
        {
            fm = fopen(argv[1], "rt");
            if(fm != NULL)
            {
                fgets(battery, 6, fm);
                fclose(fm);
                fm = NULL;
            }
            else
            {
                printf("无法读取%s文件，程序强制退出！\n", argv[1]);
                exit(2);
            }
            fq = fopen("/sys/class/power_supply/battery/status", "rt");
            if(fq != NULL)
            {
                fgets(charge_status, 20, fq);
                fclose(fq);
                fq = NULL;
            }
            else
            {
                printf("无法读取/sys/class/power_supply/battery/status文件，程序强制退出！\n");
                exit(10);
            }
            line_feed(charge_status);
            power = atoi(battery)+50;
            if(power > 9999)
            {
                if(strcmp(charge_status, "Charging") == 0)
                {
                    if(full)
                    {
                        snprintf(battery, 4, "100");
                    }
                    else
                    {
                        fp = fopen("/sys/class/power_supply/bms/current_now", "rt");
                        if(fp != NULL)
                        {
                            fgets(current_char, 20, fp);
                            fclose(fp);
                            fp = NULL;
                        }
                        else
                        {
                            printf("无法读取/sys/class/power_supply/bms/current_now文件，程序强制退出！\n");
                            exit(1);
                        }
                        current = atoi(current_char);
                        full=(current < 0)?0:1;
                        (full)?snprintf(battery, 4, "100"):snprintf(battery, 3, "99");
                    }
                }
                else
                {
                    (full)?snprintf(battery, 4, "100"):snprintf(battery, 3, "99");
                }
            }
            else
            {
                full = 0;
                if(power > 999)
                {
                    snprintf(battery, 3, "%d", power);
                }
                else if(power > 99)
                {
                    snprintf(battery, 2, "%d", power);
                }
                else
                {
                    snprintf(battery, 2, "0");
                }
            }
            set_value("/sys/class/power_supply/battery/capacity", battery);
            sleep(5);
        }
    }
    else
    {
        printf("不支持的真实电量文件路径：%s，程序强制退出！\n", argv[1]);
        exit(6);
    }
    return 0;
}
