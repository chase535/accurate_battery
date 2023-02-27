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
 - 可选择是否将涓流充电过程加入电量统计
 - 模块可通过创建或删除/data/adb/accurate_battery/no_trickle文件来更改选择
 - 创建为不加入，删除为加入，更改实时生效，无需重启手机
 - 亦可通过重刷模块来更改选择，也无需重启手机

 ********************************************************

 - 程序只是干掉UI快充，并不是校准电池容量或让电量计算更为精准
 - 电量和电池容量的计算是硬件上的事，系统和软件只能读取
 - 所以如果电池不太行了，即使用了模块，也依旧会出现10%瞬间到0%这种情况

 ********************************************************
 "
}

check_file()
{
    ui_print ""
    ui_print "--- 检查必要文件是否存在 ---"
    if [[ ! -f "/sys/class/power_supply/battery/status" || ! -f "/sys/class/power_supply/battery/current_now" || ! -f "/sys/class/power_supply/battery/capacity" ]]; then
        ui_print " ！缺少必要文件，不支持此手机，安装失败！"
        ui_print ""
        exit 1
    fi
    if [[ -f /sys/class/power_supply/bms/real_capacity ]]; then
        ui_print "- 检测到/sys/class/power_supply/bms/real_capacity文件存在，使用此文件作为真实电量文件"
        sed -i 's|^capacity_file=.*|capacity_file=/sys/class/power_supply/bms/real_capacity|g' $TMPDIR/service.sh
    elif [[ -f /sys/class/power_supply/bms/capacity_raw ]]; then
        ui_print "- 检测到/sys/class/power_supply/bms/capacity_raw文件存在，使用此文件作为真实电量文件"
        sed -i 's|^capacity_file=.*|capacity_file=/sys/class/power_supply/bms/capacity_raw|g' $TMPDIR/service.sh
    else
        ui_print " ！缺少必要文件，不支持此手机，安装失败！"
        ui_print ""
        rm -rf $MODPATH
        exit 1
    fi
    ui_print "- 必要文件均存在，开始安装"
}

volume_keytest()
{
    ui_print "--- 音量键测试 ---"
    ui_print "  请按音量+或-键"
    (/system/bin/getevent -lc 1 2>&1 | /system/bin/grep VOLUME | /system/bin/grep " DOWN" > "$TMPDIR"/events) || return 1
    return 0
}

volume_choose()
{
    while true; do
        /system/bin/getevent -lc 1 2>&1 | /system/bin/grep VOLUME | /system/bin/grep " DOWN" > "$TMPDIR"/events
        if (`cat "$TMPDIR"/events 2>/dev/null | /system/bin/grep VOLUME >/dev/null`); then
            break
        fi
    done
    if (`cat "$TMPDIR"/events 2>/dev/null | /system/bin/grep VOLUMEUP >/dev/null`); then
        return 1
    else
        return 0
    fi
}

run_volume_key_test()
{
    if volume_keytest; then
        KEYTEST=volume_choose
        ui_print "- 音量键测试完成"
    else
        KEYTEST=false
        ui_print " ！错误：没有检测到音量键选择，默认将涓流充电过程加入电量统计"
    fi
}

run_choice()
{
    ui_print ""
    ui_print "--- 请选择是否将涓流充电过程加入电量统计（默认加入） ---"
    ui_print "  音量+键 = 将涓流充电过程加入电量统计，99%-100%充电会极为缓慢"
    ui_print "  音量-键 = 不将涓流充电过程加入电量统计，100%后仍会有充电电流"
    ui_print ""
    if "$KEYTEST"; then
        ui_print "- 不将涓流充电过程加入电量统计"
        touch /data/adb/accurate_battery/no_trickle
        ui_print ""
        ui_print " ********************************************************"
        ui_print ""
        ui_print " - 注意！由于此选项设计为不将涓流充电过程加入电量统计"
        ui_print " - 所以充到100%后仍会有充电电流！"
        ui_print " - 如想将涓流充电过程加入电量统计"
        ui_print " - 请选择另一选项或删除/data/adb/accurate_battery/no_trickle文件"
        ui_print ""
        ui_print " ********************************************************"
        ui_print ""
    else
        ui_print "- 将涓流充电过程加入电量统计"
        [[ -e /data/adb/accurate_battery/no_trickle ]] && rm -rf /data/adb/accurate_battery/no_trickle
        ui_print ""
        ui_print " ********************************************************"
        ui_print ""
        ui_print " - 注意！由于此选项设计为涓流充电结束后才会变为100%"
        ui_print " - 所以99%-100%充电会极为缓慢！"
        ui_print " - 如不想将涓流充电过程加入电量统计"
        ui_print " - 请选择另一选项或创建/data/adb/accurate_battery/no_trickle文件"
        ui_print ""
        ui_print " ********************************************************"
        ui_print ""
    fi
}

on_install()
{
    cp -f $TMPDIR/accurate_battery $MODPATH/accurate_battery
    mkdir -p /data/adb/accurate_battery
    run_volume_key_test
    run_choice
}

set_permissions()
{
    set_perm_recursive  $MODPATH  0  0  0755  0644
    chmod 0777 $MODPATH/accurate_battery
    chmod 0755 /data/adb/turbo-charge
    [[ -e /data/adb/accurate_battery/no_trickle ]] && chmod 0644 /data/adb/accurate_battery/no_trickle
}

check_file
source $TMPDIR/module.prop
