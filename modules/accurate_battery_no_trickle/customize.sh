SKIPMOUNT=false
PROPFILE=false
POSTFSDATA=false
LATESTARTSERVICE=true

print_modname(){
ui_print "
 ********************************************************
 
 - 模块: $MODNAME
 - 模块ID: $MODID
 - 作者: $MODAUTHOR
 -      模块介绍↓
 - 使用C语言编写，GCC编译器静态编译
 - 精准电量，干掉虚假的UI电量
 - 电量判断至小数点后一位且采用四舍五入变为整数
 - 程序很简单，开源地址：https://github.com/chase535/accurate_battery

 ********************************************************

 - 注意！由于此版本设计为不将涓流充电过程加入电量统计
 - 所以充到100%后仍会有充电电流！
 - 如想将涓流充电过程加入电量统计，请使用后缀不带no_trickle的版本！

 ********************************************************
 "
}

on_install(){
    cp -af $TMPDIR/accurate_battery_no_trickle $MODPATH/accurate_battery_no_trickle
}

set_permissions(){
    set_perm_recursive  $MODPATH  0  0  0777  0777
}
