SKIPMOUNT=false
PROPFILE=false
POSTFSDATA=false
LATESTARTSERVICE=true

echo ' ********************************************************'
echo ''
echo " - 模块: $MODNAME"
echo " - 模块ID: $MODID"
echo " - 作者: $MODAUTHOR"
echo ' -      模块介绍↓'
echo ' - 使用C语言编写，GCC编译器静态编译'
echo ' - 精准电量，干掉虚假的UI电量'
echo ' - 程序很简单，开源地址：https://github.com/chase535/accurate_battery'
echo ''
echo ' ********************************************************'
echo ''

if [[ -f /sys/class/power_supply/bms/capacity_raw ]]; then
    echo 'capacity_file=/sys/class/power_supply/bms/capacity_raw' >> $TMPDIR/service.sh
elif [[ -f /sys/class/power_supply/bms/real_capacity ]]; then
    echo ' - 检测到/sys/class/power_supply/bms/real_capacity文件存在'
    echo ' - 将直接从此文件中读取电量，充到100%后仍会有充电电流！'
    echo ''
    echo ' ********************************************************'
    echo 'capacity_file=/sys/class/power_supply/bms/real_capacity' >> $TMPDIR/service.sh
    real_capacity="1"
else
    echo '未找到真实电量文件，不支持此手机，模块刷入失败！'
    exit 1
fi

if [[ -f $TMPDIR/accurate_battery ]]; then
    if [[ "$real_capacity" != "1" ]]; then
        echo ' - 注意！由于此版本设计为涓流充电结束后才会变为100%'
        echo ' - 所以99%-100%充电会极为缓慢！'
        echo ' - 如不想将涓流充电过程加入电量统计，请使用后缀带no_trickle的版本！'
        echo ''
        echo ' ********************************************************'
    fi
    echo 'nohup $MODDIR/accurate_battery $capacity_file &' >> $TMPDIR/service.sh
    cp -af $TMPDIR/accurate_battery $MODPATH/accurate_battery
elif [[ -f $TMPDIR/accurate_battery_no_trickle ]]; then
    if [[ "$real_capacity" != "1" ]]; then
        echo ' - 注意！由于此版本设计为不将涓流充电过程加入电量统计'
        echo ' - 所以充到100%后仍会有充电电流！'
        echo ' - 如想将涓流充电过程加入电量统计，请使用后缀不带no_trickle的版本！'
        echo ''
        echo ' ********************************************************'
    fi
    echo 'nohup $MODDIR/accurate_battery_no_trickle $capacity_file &' >> $TMPDIR/service.sh
    cp -af $TMPDIR/accurate_battery_no_trickle $MODPATH/accurate_battery_no_trickle
else
    echo '缺少主程序，模块刷入失败！'
    exit 1
fi
