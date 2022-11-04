# CMake 最低版本号要求
cmake_minimum_required (VERSION 3.24)

# 项目信息
project (accurate_battery)

# 配置环境
set(CMAKE_SYSTEM_NAME Linux)
set(CMAKE_SYSTEM_PROCESSOR aarch64)

# 交叉编译器
set(CMAKE_C_COMPILER ${CMAKE_CURRENT_SOURCE_DIR}/gcc-arm-10.3-2021.07-x86_64-aarch64-none-linux-gnu/bin/aarch64-none-linux-gnu-gcc)

# 静态编译
set(CMAKE_EXE_LINKER_FLAGS "-static -static-libgcc")

# 指定生成目标
file(GLOB_RECURSE ALL_SRCS "accurate_battery*.c")
foreach(mainfile IN LISTS ALL_SRCS)
    get_filename_component(mainname ${mainfile} NAME_WE)
    add_executable(${mainname} ${mainfile})
endforeach()
