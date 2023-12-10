#!/system/bin/sh
MODDIR=${0%/*}
capacity_file=capacity_file_not_defined
[[ -f /data/adb/accurate_battery/log.txt ]] && mv -f /data/adb/accurate_battery/log.txt /data/adb/accurate_battery/log.txt.old
echo "等待手机启动完毕，以确保时间准确。若只看到这一行内容，请立即联系模块作者！" > /data/adb/accurate_battery/log.txt
until [[ "$(getprop service.bootanim.exit)" == "1" ]]; do
    sleep 1
done
echo "手机启动完毕" >> /data/adb/accurate_battery/log.txt
echo "" >> /data/adb/accurate_battery/log.txt
nohup ${MODDIR}/accurate_battery ${capacity_file} >> /data/adb/accurate_battery/log.txt 2>&1 &
