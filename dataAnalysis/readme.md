## 环境

python 3.6

packages

>   pandas, scipy, seabornd, matplotlib

## 文件说明

1.  `main.py` 包含处理 `demo` 文件夹下4个`txt` 文件的代码。运行后在 `demo` 目录下生成 `_tilt.png` 结尾的图片，第一幅子图是 y 轴加速度，第二幅子图是加速度变化率，第三幅子图是杯身倾斜角度。另外会打印估计的喝水临界点和结束点角度，第一行是弧度值，第二行是对应的度数值。
2.   `demo` 文件夹包含4个`txt` 文件，分别对应mpu6050采集的4次喝水数据，每行三个数据分别代表 x、y、z轴加速度，其中 y 轴代表杯身方向，另外两轴数据仅用于辅助计算杯身相对竖直方向的倾斜角度。

3.  `手机传感器测量数据` 文件夹包含已经画好图的用手机测量的喝水数据。
4.  `baud115200` 文件夹下是将波特率由9600调到115200后采集到的喝水数据，已做好图。
5.  `img `文件夹下是开发过程中保存的图片