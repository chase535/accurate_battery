# CMake 最低版本号要求
cmake_minimum_required(VERSION 2.8.12)

# 项目信息
project(accurate_battery)

# 配置环境
set(CMAKE_SYSTEM_NAME "Linux")
set(CMAKE_SYSTEM_PROCESSOR "aarch64")



# 指定生成目标
file(GLOB_RECURSE ALL_SRCS "accurate_battery*.c")
foreach(mainfile IN LISTS ALL_SRCS)
    get_filename_component(mainname ${mainfile} NAME_WE)
    add_executable(${mainname} ${mainfile})
endforeach()
