# accurate_battery
精准电量，干掉虚假的UI电量，酷安@大鸟转转转酒吧

电量判断至小数点后一位且采用四舍五入变为整数

精准电量，干掉虚假的UI电量，安装时按音量键选择是否将涓流充电过程加入电量统计

模块可通过创建或删除/data/adb/accurate_battery/no_trickle文件来更改选择，**创建为不加入，删除为加入**，更改实时生效，无需重启手机，亦可通过重刷模块来更改选择，也无需重启手机

程序**只是**干掉UI快充，并不是校准电池容量或让电量计算更为精准，电量和电池容量的计算是硬件上的事，系统和软件只能读取，所以如果电池不太行了，即使用了模块，也**依旧会出现10%瞬间到0%这种情况**

使用自编译的aarch64-linux-musl-gcc，项目地址：[aarch64-linux-musl-gcc](https://github.com/chase535/aarch64-linux-musl-gcc)

已经开启CI构建，可在Actions页面下载CI构建版本以获取最新测试版，**不保证CI构建版本的功能性及稳定性**

**程序遵循AGPLv3开源协议**
