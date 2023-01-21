SKIPMOUNT=false
PROPFILE=false
POSTFSDATA=false
LATESTARTSERVICE=true

echo ' ********************************************************'
echo ''
echo " - 模块: $MODNAME"
echo " - 模块ID: $MODID"
echo " - 模块版本: $MODversion"
echo " - 作者: $MODAUTHOR"
echo ' -      模块介绍↓'
echo ' - 使用C语言编写，Github Actions进行静态交叉编译(CMake + aarch64-linux-musl-gcc)'
echo ' - 精准电量，干掉虚假的UI电量'
echo ' - 程序很简单，开源地址：https://github.com/chase535/accurate_battery'
echo ''
echo ' ********************************************************'
echo ''

if [[ -f /sys/class/power_supply/bms/real_capacity ]]; then
    echo ' - 检测到/sys/class/power_supply/bms/real_capacity文件存在'
    sed -i 's|^capacity_file=.*|capacity_file=/sys/class/power_supply/bms/real_capacity|g' $TMPDIR/service.sh
elif [[ -f /sys/class/power_supply/bms/capacity_raw ]]; then
    echo ' - 检测到/sys/class/power_supply/bms/capacity_raw文件存在'
    sed -i 's|^capacity_file=.*|capacity_file=/sys/class/power_supply/bms/capacity_raw|g' $TMPDIR/service.sh
else
    echo '未找到真实电量文件，不支持此手机，模块刷入失败！'
    exit 1
fi

