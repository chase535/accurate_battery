#include <stdio.h>
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
    }
}

int main()
{
    FILE *fm;
    char battery[4];
    int power;
    while(1)
    {
        memset(battery, '\0', sizeof(battery));
        fm = fopen("/sys/class/power_supply/bms/capacity_raw", "rt");
        if(fm != NULL)
        {
            fscanf(fm, "%d", &power);
            power += 50;
            if(power>9999)
            {
                sprintf(battery, "100");
            }
            else if(power>999)
            {
                snprintf(battery, 3, "%d", power);
            }
            else if(power>99)
            {
                snprintf(battery, 2, "%d", power);
            }
            else
            {
                sprintf(battery, "0");
            }
            set_value("/sys/class/power_supply/battery/capacity", battery);
            fclose(fm);
            fm = NULL;
        }
        sleep(15);
    }
    return 0;
}
