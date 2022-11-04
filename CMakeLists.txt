# CMake 最低版本号要求
cmake_minimum_required (VERSION 2.8)

# 项目信息
project (accurate_battery)

# 指定生成目标
add_executable(accurate_battery accurate_battery.c)

set(CMAKE_EXE_LINKER_FLAGS "-static")

set(CMAKE_SYSTEM_NAME Linux)
set(CMAKE_SYSTEM_PROCESSOR aarch64)

# 交叉编译器
set(CMAKE_C_COMPILER ${{github.workspace}}/gcc-arm-10.3-2021.07-x86_64-aarch64-none-linux-gnu/bin/aarch64-none-linux-gnu-gcc)
