#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "unistd.h"
#include "sys/types.h"
#include "sys/stat.h"

void set_value(char *file, char *numb)
{
    FILE *fn;
    if(access(file, F_OK) == 0)
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
            chmod(file, 0644);
            fn = fopen(file, "wt");
            if(fn != NULL)
            {
                fputs(numb, fn);
                fclose(fn);
                fn = NULL;
            }
            else
            {
                printf("无法向%s文件写入数据，程序强制退出！\n", file);
                exit(3);
            }
        }
    }
    else
    {
        printf("找不到%s文件，程序强制退出！\n", file);
        exit(4);
    }
}

void line_feed(char *line)
{
    char *p;
    p=strchr(line, '\n');
    if(p != NULL) *p='\0';
}

void check_file(char *file)
{
    if(access(file, F_OK) != 0)
    {
        printf("无法找到%s文件，程序强制退出！\n", file);
        exit(999);
    }
}

void read_file(char *file,char *value)
{
    FILE *fp;
    fp = fopen(file, "rt");
    if(fp != NULL)
    {
        fgets(value, sizeof(value), fp);
        line_feed(value);
        fclose(fp);
        fp = NULL;
    }
    else
    {
        printf("无法读取%s文件，程序强制退出！\n", file);
        exit(30);
    }
}

int main(int argc, char *argv[])
{
    char battery[10],current_char[30],charge_status[25];
    int power,current,full=0,num=0,no_trickle,charge_full;
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
    else if(strcmp(argv[1], "/sys/class/power_supply/bms/real_capacity") != 0 && strcmp(argv[1], "/sys/class/power_supply/bms/capacity_raw") != 0)
    {
        printf("不支持的真实电量文件路径：%s，程序强制退出！\n", argv[1]);
        exit(6);
    }
    check_file("/sys/class/power_supply/battery/status");
    check_file("/sys/class/power_supply/battery/current_now");
    check_file("/sys/class/power_supply/battery/capacity");
    check_file(argv[1]);
    charge_full=(strcmp(argv[1], "/sys/class/power_supply/bms/real_capacity") == 0)?100:10000;
    while(1)
    {
        read_file("/sys/class/power_supply/battery/current_now", current_char);
        current = atoi(current_char);
        read_file("/sys/class/power_supply/battery/status", charge_status);
        no_trickle=(access("/data/adb/accurate_battery/no_trickle", F_OK) == 0)?1:0;
        if(strcmp(charge_status, "Full") == 0) num=(current <= 0)?num+1:0;
        else num=0;
        if(num > 5)
        {
            num=5;
            full=1;
            read_file(argv[1], battery);
            charge_full=atoi(battery);
            set_value("/sys/class/power_supply/battery/capacity", "100");
            continue;
        }
        if(strcmp(argv[1], "/sys/class/power_supply/bms/real_capacity") == 0)
        {
            read_file(argv[1], battery);
            power=atoi(battery)+100-charge_full;
            if(!no_trickle)
            {
                if(power >= 100)
                {
                    if(!full) snprintf(battery, 3, "99");
                }
                else full=0;
            }
        }
        else if(strcmp(argv[1], "/sys/class/power_supply/bms/capacity_raw") == 0)
        {
            read_file(argv[1], battery);
            power=atoi(battery)+10000-charge_full+50;
            if(power > 9999)
            {
                if(no_trickle || full) snprintf(battery, 4, "100");
                else snprintf(battery, 3, "99");
            }
            else
            {
                full=0;
                if(power > 999) snprintf(battery, 3, "%d", power);
                else if(power > 99) snprintf(battery, 2, "%d", power);
                else snprintf(battery, 2, "0");
            }
        }
        set_value("/sys/class/power_supply/battery/capacity", battery);
        sleep(5);
    }
    return 0;
}
