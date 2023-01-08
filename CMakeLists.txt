# CMake 最低版本号要求
cmake_minimum_required(VERSION 2.8.12)

# 项目信息
project(accurate_battery)

# 配置环境
set(CMAKE_SYSTEM_NAME "Linux")
set(CMAKE_SYSTEM_PROCESSOR "aarch64")

# 指定生成目标
add_executable(accurate_battery accurate_battery.c)
add_executable(accurate_battery_no_trickle accurate_battery_no_trickle.c)
