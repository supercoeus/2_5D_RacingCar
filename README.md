# 2.5D RacingCar

**原作者：极品史莱姆 (799052200@qq.com)**

原作地址：https://codebus.cn/contributor/jpslm-2d-racing-car

这是原作者的《2D 赛车》的修改版——2.5D 双人赛车！

## 编译环境

Visual Studio 2022

Windows 10

EasyX 20211109

## 游戏截图

![图片](https://github.com/zouhuidong/2_5D_RacingCar/blob/main/screenshot/2.png)

![图片](https://github.com/zouhuidong/2_5D_RacingCar/blob/main/screenshot/3.png)

![图片](https://github.com/zouhuidong/2_5D_RacingCar/blob/main/screenshot/1.png)

**原作游戏截图**

![图片](https://github.com/zouhuidong/2_5D_RacingCar/blob/main/screenshot/4.png)

## 更新日志

**Ver 1.1**

游戏内容方面：

1. 优化了小车坐标旋转算法，速度提示 60 倍以上。（详见 `GetRotatedCarPosition` 和 `GetRotatedCarPosition2` 函数）

2. 新增低分辨率模式，以适配小显示器。

3. 车速提高时转向速度相应提高。

4. 对齐地图 b 掩码图中左上方第一个沙坑的位置。

代码方面：

1. 新增文件 NewDrawer.h，将绘图功能模块独立出来。

2. 原作中将按钮结构体的名称 `button` 错拼为 `bottom`，已更正，并移除其成员 a。

3. 更正原作中双人模式胜利判定逻辑。

4. 优化 `Loading` 函数中按钮选择的判断逻辑。

5. 修改原窗体宽度变量名 `WIDE` 为 `WIDTH`。

**Ver 1.0** (2021.12.25)

1. 视角改为 2.5D（伪 3D）

2. 新增 SHIFT 键操作

3. 修改了速度等部分参数

4. 显示小地图

5. 显示速度

## 想法

作为一名忙碌的学生党……在周六回家的时候挤了一点时间做了这个修改版。
我感觉原作做的已经很好，但是我玩着玩着就觉得这个游戏和我们一般玩的赛车游戏有点点不对味，
因为我们玩的赛车游戏一般都是第一人称的，不会看到整张地图来开车……
所以我就心血来潮做了这个修改版。

我通过跟随游戏中小车的视角，把第一人称的视角在地图中获取下来，并通过“近大远小”的原理，
模拟出 3D 的感觉，所以这是个伪 3D 游戏，我就把它称作“2.5D”了。

跟随小车的视角，其实只需要把小车绘制到地图上，再把整个地图转到和小车相同的角度，这样小车所朝方向就是正方向了，
然后再从旋转后的地图上，从小车的位置开始，往前获取一段图像就可以了（当然还要做“近大远小”的透视效果）。

原理其实很简单，但是真正上手做的时候有一个需要注意的地方，也就是随着整张地图的旋转，小车在地图上的坐标也要转动，
一开始我没想到好的解法，也没太多时间（因为周末的时候），于是我干脆就用了一个简单粗暴的办法，就在一个空白的 IMAGE 对象的小车的位置画一个矩形，
再旋转整个 IMAGE 到地图旋转角，这样我再在 IMAGE 对象上找到矩形的坐标就知道小车的坐标了。
这个办法笨笨的……不过在 Ver 1.1 中使用了新的算法直接算出了小车的坐标，速度大为提升。

除此之外，由于 2D 转 “3D” 的过程需要对 IMAGE 对象逐条线段取样，比较耗时，导致游戏稍有卡顿，暂时无法对其进行进一步的优化……不过还是很好玩的。

嗯……玩的开心吧！
