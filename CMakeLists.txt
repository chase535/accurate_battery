# CMake最低版本号要求
cmake_minimum_required(VERSION 3.14)

# 项目信息
project(accurate_battery)

# 配置环境
set(CMAKE_SYSTEM_NAME "Linux")
set(CMAKE_SYSTEM_PROCESSOR "aarch64")

# 发行版本
set(CMAKE_BUILD_TYPE Release)

# 设置编译链接参数
set(CMAKE_C_FLAGS "-g0 -O2 -W -Wall -Wno-format-truncation -Wno-unused-result -Werror -fvisibility=hidden -flto -fno-omit-frame-pointer -fdata-sections -ffunction-sections")
set(CMAKE_EXE_LINKER_FLAGS "-g0 -O2 -static -fvisibility=hidden -flto -ffixed-x18 -Wl,-O2,--strip-all,--hash-style=both,--gc-sections,--exclude-libs,ALL")

# 指定生成目标
add_executable(accurate_battery accurate_battery.c)
