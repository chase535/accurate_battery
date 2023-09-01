SKIPUNZIP=1

check_file()
{
    ui_print " "
    ui_print "--- 检查必要文件是否存在 ---"
    if [[ ! -f "/sys/class/power_supply/battery/status" || ! -f "/sys/class/power_supply/battery/current_now" || ! -f "/sys/class/power_supply/battery/capacity" ]]; then
        ui_print " ！缺少必要文件，不支持此手机，安装失败！"
        ui_print " "
        rm -rf ${MODPATH}
        exit 1
    fi
    if [[ -f /sys/class/power_supply/bms/real_capacity ]]; then
        ui_print "- 检测到/sys/class/power_supply/bms/real_capacity文件存在，使用此文件作为真实电量文件"
        sed -i 's|^capacity_file=.*|capacity_file=/sys/class/power_supply/bms/real_capacity|g' ${TMPDIR}/service.sh
    elif [[ -f /sys/class/power_supply/bms/capacity_raw ]]; then
        ui_print "- 检测到/sys/class/power_supply/bms/capacity_raw文件存在，使用此文件作为真实电量文件"
        sed -i 's|^capacity_file=.*|capacity_file=/sys/class/power_supply/bms/capacity_raw|g' ${TMPDIR}/service.sh
    else
        ui_print " ！缺少必要文件，不支持此手机，安装失败！"
        ui_print " "
        rm -rf ${MODPATH}
        exit 1
    fi
    ui_print "- 必要文件存在，开始安装"
}

print_modname()
{
    ui_print " "
    ui_print " ********************************************************"
    ui_print " "
    ui_print " - 模块: $(grep '^name=' ${TMPDIR}/module.prop | sed 's/^name=//g')"
    ui_print " - 模块版本: $(grep '^version=' ${TMPDIR}/module.prop | sed 's/^version=//g')"
    ui_print " - 作者: $(grep '^author=' ${TMPDIR}/module.prop | sed 's/^author=//g')"
    ui_print " "
    ui_print " ********************************************************"
    ui_print " "
}

print_info()
{
    ui_print " "
    ui_print " ********************************************************"
    ui_print " "
    ui_print " -      ↓模块介绍↓"
    ui_print " - 精准电量，干掉虚假的UI电量"
    ui_print " - 可选择是否将涓流充电过程加入电量统计"
    ui_print " - 模块可通过创建或删除/data/adb/accurate_battery/no_trickle文件来更改选择"
    ui_print " - 创建为不加入，删除为加入，更改实时生效，无需重启手机"
    ui_print " - log文件为同目录的log.txt"
    ui_print " "
    ui_print " ********************************************************"
    ui_print " "
    ui_print " - 程序只是干掉UI快充，并不是校准电池容量或让电量计算更为精准"
    ui_print " - 电量和电池容量的计算是硬件上的事，系统和软件只能读取"
    ui_print " - 所以如果电池不太行了，即使用了模块，也依旧会出现10%瞬间到0%这种情况"
    ui_print " "
    ui_print " ********************************************************"
    ui_print " "
}

on_install()
{
    cp -af ${TMPDIR}/accurate_battery ${TMPDIR}/module.prop ${TMPDIR}/service.sh ${TMPDIR}/uninstall.sh ${MODPATH}
    [[ ! -d /data/adb/accurate_battery ]] && mkdir -p /data/adb/accurate_battery
    if [[ -f /data/adb/accurate_battery/no_trickle ]]; then
        ui_print "- /data/adb/accurate_battery/no_trickle文件存在"
        ui_print "- 不将涓流充电过程加入电量统计"
        ui_print " "
        ui_print " - 注意！此选项充到100%后仍会有充电电流！"
        ui_print " - 如想将涓流充电过程加入电量统计"
        ui_print " - 请删除/data/adb/accurate_battery/no_trickle文件"
        ui_print " "
        ui_print " ********************************************************"
        ui_print " "
    else
        ui_print "- /data/adb/accurate_battery/no_trickle文件不存在"
        ui_print "- 将涓流充电过程加入电量统计"
        ui_print " "
        ui_print " - 注意！由于此选项设计为涓流充电结束后才会变为100%"
        ui_print " - 所以99%-100%充电会极为缓慢！"
        ui_print " - 如不想将涓流充电过程加入电量统计"
        ui_print " - 请创建/data/adb/accurate_battery/no_trickle文件"
        ui_print " "
        ui_print " ********************************************************"
        ui_print " "
    fi
}

set_permissions()
{
    set_perm_recursive  ${MODPATH}  0  0  0755  0777
    set_perm_recursive  /data/adb/accurate_battery  0  0  0755  0777
}

unzip -oj "${ZIPFILE}" module.prop uninstall.sh 'common/*' -d ${TMPDIR} >&2
print_modname
check_file
print_info
on_install
set_permissions
