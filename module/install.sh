SKIPMOUNT=false
PROPFILE=false
POSTFSDATA=false
LATESTARTSERVICE=true

print_modname()
{
ui_print "
 ********************************************************
 
 - 模块: $name
 - 模块版本: $version
 - 作者: $author
 -      ↓模块介绍↓
 - 精准电量，干掉虚假的UI电量

 ********************************************************

 - 程序只是干掉UI快充，并不是校准电池容量或让电量计算更为精准
 - 电量和电池容量的计算是硬件上的事，系统和软件只能读取
 - 所以如果电池不太行了，即使用了模块，也依旧会出现10%瞬间到0%这种情况

 ********************************************************
 "
}

check_file()
{
    if [[ -f /sys/class/power_supply/bms/real_capacity ]]; then
        ui_print " - 检测到/sys/class/power_supply/bms/real_capacity文件存在"
        sed -i 's|^capacity_file=.*|capacity_file=/sys/class/power_supply/bms/real_capacity|g' $TMPDIR/service.sh
    elif [[ -f /sys/class/power_supply/bms/capacity_raw ]]; then
        ui_print " - 检测到/sys/class/power_supply/bms/capacity_raw文件存在"
        sed -i 's|^capacity_file=.*|capacity_file=/sys/class/power_supply/bms/capacity_raw|g' $TMPDIR/service.sh
    else
        ui_print " ！未找到真实电量文件，不支持此手机，模块刷入失败！"
        ui_print ""
        exit 1
    fi
}
