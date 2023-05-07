<<<<<<< HEAD
# RP_2023_EC

底层驱动主要来自马哥的代码

主要写了设备部分，理论上可以实现无痛开发

-----------------------------------------------
注意：
使用cube后，需要用user中的cmsis文件替换drives中的cmsis

-----------------------------------------------

代码主要内容

BEST_BLESS_FOR_RPers
自带护身符文件，保佑代码无bug

-----------------------------------------------

RP_CONFIG：
全局宏定义
 设置开启全局CAN通信、USART通信
 一键设置视觉、裁判系统的通信串口

测试代码：
 电机测试代码
 设置好电机信息:通信方式、id、电机类型
 pid需要自己设置在motor.c中设置
 可以让电机匀速旋转

串口测试代码
 接入串口后直接使用串口助手，复读机功能

可选择主控
 设置陀螺仪的装配姿态、通信方式、pid信息

-----------------------------------------------

RP_INIT：
驱动以及设备初始化
 自动执行iic的spi或者iic通信初始化
 串口初始化
 can初始化
 电机初始化
 
-----------------------------------------------

Tasks_Center：
 任务中心
  监测任务
  通信任务
  控制任务
  
-----------------------------------------------

DEVICES：
（文件都尽量提高了移植性，可以选择自己需要的移植）
设备的类型定义：
info：
设备相关的信息
状态通信等
data：
和设备相关的数据
操作函数：
各种操作函数


rc：
  键盘状态检测（已添加监测任务中，可宏定义关闭）
  每个按键带标志位，便于统计按下次数等
  拨轮可以设置四个阶跃信号，每次滚轮只检测一次，为到达的最大值的信号，归中时产生信号
  带拨杆苏醒标志，当开机后拨杆都归中时，才会解锁拨杆，该标志位对拨扭和拨轮无效
  带wsad模拟摇杆，可以设置响应速度
  目前不需要改动立刻可以使用
  
led：
  可以设置闪烁以及常亮
  大主控没有适配

cap：
  可以自动接收信息
  使用需要自己添加设置信息文件 函数cap.setdata
  再发送0x2E、0x2F的信息包   

master：
 定义并且设定多个主控的信息
 添加主控无需添加心跳包
 数据类型需要自己在master.h中添加
 发生和接收信息需要在下面的函数中添加

imu：
  重新移植了bmi270的文件，只需要移植4个文件
  添加spi和iic的通信方式切换，但某些大主控无法实现spi通信，
  spi数据获取一次在100us内，iic数据获取一次需要500us左右
  
  可以设置imu的装配坐标，修改正确的状态
  
  可以设置rpy解算、世界角速度解算、校正解算
  
  世界角速度解算可以设置为：
   用四元数解算imu测量的角速度
   使用角度差分计算的角速度
   （使用角度差分的云台控制貌似效果会好点）
  
  校正解算可以设定校正时间，默认使用1s，测量的速度过大会自动剔除
  
  含四通道低通滤波器，直接设定滤波强度就行，实际上只需要给加速度滤波（测试最好的滤波系数为0.27，忘记是a还是1-a了）

  imu.c中有旋转矩阵左乘右乘的函数，以及欧拉角四元数互换的函数，可以用于姿态转换
  
rm_motor：
  全部大疆电机包，包括3508、6020、3510电机
  自带速度、角度、位置pid控制、以及自定义pid控制
  pid函数可以设定err的处理方式，设定不处理、半圈处理、1/4圈处理，用于不同情况
  无需改动初始化、接收、心跳函数
  
  在motor.c中定义：
  使用步骤：
  1.定义电机名称、类型、通信方式、ID
  2.定义pid数据组
  3.初始化pid信息
  4.定义发送数组发送
  发送可以使用内置tx函数，内检测数组全为0不发送、发送后清零
  
vision：
  需要自己改协议
  需要自己发送数据

judge：
  需要自己定义和发送UI数据

potocal：
 通信的接收集
 can
 usart






















=======
# RP_2023_EC
>>>>>>> f8faad481222213e547157e2f5e8a8e9aafc8b65