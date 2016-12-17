#bpi-ledtrig-temperature

香蕉派Banana Pi M3 做的led trigger 练习

参考了原来的heartbeat trigger, cpu温度越高, led闪烁得越快

## 编译和使用

1. 使用交叉编译, 系统ubuntu12.04, 安装gcc-arm-linux-gnueabihf
2. 依赖bpi的linux内核[BPI_M3-bsp](https://github.com/BPI-SINOVOIP/BPI-M3-bsp), 修改Makefile相应的KERNEL_DIR 值为存放地址
3. make 编译, 拷贝到pi 里
4. 在pi 上执行模块加载:

	```bash
	$ sudo insmod -f ledtrig-temperature.ko
	```

5. 将绿色led切换到 这个trigger:

	```bash
	$ sudo bash -c 'echo "cpu_temp" >/sys/class/leds/green_leds/trigger'
	```


