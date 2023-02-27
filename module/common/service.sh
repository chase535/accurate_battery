#!/system/bin/sh
MODDIR=${0%/*}
capacity_file=capacity_file_not_defined
nohup ${MODDIR}/accurate_battery ${capacity_file} &
