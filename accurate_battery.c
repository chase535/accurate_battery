#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "time.h"
#include "unistd.h"
#include "stdarg.h"
#include "sys/types.h"
#include "sys/stat.h"

void get_utc8_time(struct tm *ptm)
{
    time_t cur_time;
    time(&cur_time);
    memcpy(ptm, gmtime(&cur_time), sizeof(struct tm));
    ptm->tm_year+=1900;
    ptm->tm_mon+=1;
    ptm->tm_hour+=8;
    if(ptm->tm_hour > 23)
    {
        ptm->tm_hour-=24;
        ptm->tm_mday+=1;
        switch(ptm->tm_mon)
        {
            case 1: case 3: case 5: case 7: case 8: case 10: case 12:
                if(ptm->tm_mday > 31)
                {
                    ptm->tm_mday-=31;
                    ptm->tm_mon+=1;
                }
                break;
            case 2:
                if(((ptm->tm_year%4 == 0) && (ptm->tm_year%100 != 0)) || (ptm->tm_year%400 == 0))
                {
                    if(ptm->tm_mday > 29)
                    {
                        ptm->tm_mday-=29;
                        ptm->tm_mon+=1;
                    }
                }
                else
                {
                    if(ptm->tm_mday > 28)
                    {
                        ptm->tm_mday-=28;
                        ptm->tm_mon+=1;
                    }
                }
                break;
            default:
                if(ptm->tm_mday > 30)
                {
                    ptm->tm_mday-=30;
                    ptm->tm_mon+=1;
                }
                break;
        }
        if(ptm->tm_mon > 12)
        {
            ptm->tm_mon-=12;
            ptm->tm_year+=1;
        }
    }
}

void printf_with_time(const char *format, ...) __attribute__((__format__(__printf__, 1, 2)));
void printf_with_time(const char *format, ...)
{
    char buffer[1024];
    struct tm time_utc8_now;
    va_list ap;
    va_start(ap, format);
    vsnprintf(buffer, 1024, format, ap);
    va_end(ap);
    get_utc8_time(&time_utc8_now);
    printf("[ %04d.%02d.%02dT%02d:%02d:%02d UTC+8 ] %s\n", time_utc8_now.tm_year, time_utc8_now.tm_mon, time_utc8_now.tm_mday, time_utc8_now.tm_hour, time_utc8_now.tm_min, time_utc8_now.tm_sec, buffer);
    fflush(stdout);
}

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
                printf_with_time("无法向%s文件写入数据，程序强制退出！", file);
                exit(3);
            }
        }
    }
    else
    {
        printf_with_time("找不到%s文件，程序强制退出！", file);
        exit(4);
    }
}

void line_feed(char *line)
{
    char *p;
    p=strchr(line, '\r');
    if(p != NULL) *p='\0';
    p=strchr(line, '\n');
    if(p != NULL) *p='\0';
}

void check_file(char *file)
{
    if(access(file, F_OK) != 0)
    {
        printf_with_time("无法找到%s文件，程序强制退出！", file);
        exit(999);
    }
}

void check_read_file(char *file)
{
    if(!access(file, F_OK))
    {
        check_permission:
        if(access(file, R_OK))
        {
            chmod(file, 0644);
            if(access(file, R_OK))
            {
                printf_with_time("无法读取%s文件，程序强制退出！", file);
                exit(1);
            }
        }
    }
    else
    {
        struct timespec req={0, 250000000L};
        nanosleep(&req, NULL);
        if(!access(file, F_OK)) goto check_permission;
        else
        {
            printf_with_time("找不到%s文件，程序强制退出！", file);
            exit(999);
        }
    }
}

void read_file(char *file_path, char *char_var, int max_char_num)
{
    FILE *fp;
    check_read_file(file_path);
    fp=fopen(file_path, "rt");
    fgets(char_var, max_char_num, fp);
    fclose(fp);
    fp=NULL;
    line_feed(char_var);
}

int main(int argc, char *argv[])
{
    char battery[10],current_char[30],charge_status[25];
    int power=0,current=0,full=0,num=0,no_trickle=100,charge_full=0;
    printf("作者：酷安@诺鸡鸭\n");
    printf("GitHub开源地址：https://github.com/chase535/accurate_battery\n\n");
    fflush(stdout);
    if(argc < 2)
    {
        printf_with_time("请传入真实电量文件路径！");
        exit(7);
    }
    else if(argc > 2)
    {
        printf_with_time("请传入真实电量文件路径，勿传入多余参数！");
        exit(20);
    }
    else if(strcmp(argv[1], "/sys/class/power_supply/bms/real_capacity") != 0 && strcmp(argv[1], "/sys/class/power_supply/bms/capacity_raw") != 0)
    {
        printf_with_time("不支持的真实电量文件路径：%s，程序强制退出！", argv[1]);
        exit(6);
    }
    check_file("/sys/class/power_supply/battery/status");
    check_file("/sys/class/power_supply/battery/current_now");
    check_file("/sys/class/power_supply/battery/capacity");
    check_file(argv[1]);
    charge_full=(strcmp(argv[1], "/sys/class/power_supply/bms/real_capacity") == 0)?100:10000;
    while(1)
    {
        read_file("/sys/class/power_supply/battery/current_now", current_char, sizeof(current_char));
        current = atoi(current_char);
        read_file("/sys/class/power_supply/battery/status", charge_status, sizeof(charge_status));
        if(access("/data/adb/accurate_battery/no_trickle", F_OK) == 0 && no_trickle != 1)
        {
            printf_with_time("/data/adb/accurate_battery/no_trickle文件存在，不将涓流充电过程加入电量统计");
            no_trickle=1;
        }
        else if(access("/data/adb/accurate_battery/no_trickle", F_OK) != 0 && no_trickle != 0)
        {
            printf_with_time("/data/adb/accurate_battery/no_trickle文件不存在，将涓流充电过程加入电量统计");
            no_trickle=0;
        }
        no_trickle=(access("/data/adb/accurate_battery/no_trickle", F_OK) == 0)?1:0;
        if(strcmp(charge_status, "Full") == 0) num=(current <= 0)?num+1:0;
        else num=0;
        if(num > 5)
        {
            num=5;
            full=1;
            read_file(argv[1], battery, sizeof(battery));
            charge_full=atoi(battery);
            set_value("/sys/class/power_supply/battery/capacity", "100");
            sleep(5);
            continue;
        }
        if(strcmp(argv[1], "/sys/class/power_supply/bms/real_capacity") == 0)
        {
            read_file(argv[1], battery, sizeof(battery));
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
            read_file(argv[1], battery, sizeof(battery));
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
