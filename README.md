# 2.5D RacingCar | 2.5D 双人赛车
![License](https://img.shields.io/github/license/zouhuidong/2_5D_RacingCar)
![Downloads](https://img.shields.io/github/downloads/zouhuidong/2_5D_RacingCar/total)
![GitHub Version](https://img.shields.io/github/v/release/zouhuidong/2_5D_RacingCar)

Table of Contents
=================

* [2\.5D RacingCar | 2\.5D 双人赛车](#25d-racingcar--25d-双人赛车)
  * [简介](#简介)
  * [编译环境](#编译环境)
  * [游戏截图](#游戏截图)
  * [“3D”原理](#3d原理)
  * [制图建议](#制图建议)
    * [地图的绘制](#地图的绘制)
    * [掩码图的绘制](#掩码图的绘制)
    * [车辆的绘制](#车辆的绘制)
  * [踩坑](#踩坑)
    * [踩坑 1](#踩坑-1)
    * [踩坑 2](#踩坑-2)
    * [踩坑 3](#踩坑-3)
    * [还有吗](#还有吗)
  * [更新日志](#更新日志)

## 简介

**原作者：极品史莱姆 (799052200@qq.com)**

原作地址：https://codebus.cn/tonyddg/2d-racing-car

这是原作者的《2D 赛车》的修改版——2.5D 双人赛车！

作为一名忙碌的学生党……我在周六回家的时候挤了一点时间做了这个修改版。

我感觉原作做的已经很好，但是不符合通常的赛车游戏习惯，因为我们玩的赛车游戏一般都是第一人称的，不会看到整张地图来开车……

所以我就心血来潮做了这个修改版。

第一个版本是在上学期间的周末做的，没有什么时间，很多还没有做好，后续的版本补充了很多。

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

## “3D”原理

我通过跟随游戏中小车的视角，把第一人称的视角在地图中获取下来，并通过“近大远小”的原理，模拟出 3D 的感觉，所以这是个伪 3D 游戏，我就把它称作“2.5D”了。

跟随小车的视角，其实只需要把小车绘制到地图上，再把整个地图转到和小车相同的角度，这样小车所朝方向就是正方向了，
然后再从旋转后的地图上，从小车的位置开始，往前获取一段图像就可以了（当然还要做“近大远小”的透视效果，见 `GetPlayerView` 函数）。

## 制图建议

如果你想要自己做地图，或者 DIY 车辆，那么你可以看看这一节。

注：我的地图项目原件在 MakingMap 目录下。

### 地图的绘制

原作是 2D 上帝视角游戏，但是这个修改版是第一人称的，所以绘制地图时需要考虑到游戏时的视角，比如说开到某个位置的时候视角是怎样的，那么这个区域的景物也得顺着玩家的视野朝向才行。

Ver 2.0 新增的 【双鸭山】 地图就是一个例子，虽然这张地图还是很有改进空间，但是作为第一张顺应 2.5D 视角的地图，也可以参考。

至于绘图软件，由于本人暂未掌握 PS 等专业绘图软件，也就会用一点 Windows 的自带绘图软件，那个东西根本没法用来画地图……所以我就选择了儿时学过的 Adobe Flash 来用。你没有看错，就是那个被时代抛弃的 Flash，作为一个优秀的动画制作软件，尽管过时，还是很好用的。

我用的是 Adobe Flash CC 2018，首先新建一个项目，调整舞台大小到目标地图大小，然后用手写板 + 铅笔绘制道路，然后利用【元件】复制各种花草树木，画出了地图。

画完地图和掩码图之后，只需要在地图和掩码图所在帧点击【文件】->【导出】->【导出图像】，选择 png 格式导出即可。

导出时需要注意的是，可能实际在 Flash 中画出来的内容超出了舞台大小，但是导出的时候会把他们算在内，导致导出的图像尺寸大于预期，这其实没关系，导出之后再裁剪就行了。

Tip: 项目目录下的 MakingMap 文件夹，有我的 Flash 项目原件。

也可以参考原作帖子 https://codebus.cn/tonyddg/2d-racing-car ，其中也谈到关于制图的问题。

### 掩码图的绘制

Flash 项目中，我画地图只用到一帧，所以画掩码图的时候在第二帧画，拉个阴影到第一帧对照着用手写板描就行了，如果想了解更多，建议看看我的 Flash 项目原件。

提几个要点：画掩码图宁可把道路画粗一点，因为实际游戏中可能会因为掩码图画的太窄导致玩家好像在路上但是被卡住了。

另外，终点线和玩家起点这些细节性的东西就不要在 Flash 中绘制了，因为导出的时候由于他们像素点太小了，色值很可能被模糊，这样游戏里面就检测不到了，所以建议导出 png 之后再用 Windows 画图来描终点线和玩家起点。

掩码图色彩格式：

1. 黑色（0xFFFFFF）为赛道，减速较小
2. 黄色（0xFFFF00）为沙地，会减速
3. 蓝色（0x0000FF）为冰面，会滑动
4. 红色（0xFF0000）为围栏，无法通过
5. 绿色（0xFFFFFF）为终点线
6. 灰色（0xAAAAAA）为终点线的两端，用于判定方向（对应色值 RGB(170,170,170)）
7. 紫色（0xFF00FF）为玩家 2 起点
8. 白色（0x000000）为玩家 1 起点

注意事项：
1. 终点线最好为宽度为一的直线！
2. 灰色只有两个像素点，分别点在终点线的两端点

此外，设置玩家起始位置也要考虑玩家车辆的长度，如果玩家的起始位置离终点线太近，以至于玩家的车尾超过了终点线的话，那么开动车子的时候就会触发一次车子越过终点的判定，这种情况当然是要避免的。

### 车辆的绘制

经测试，车身最好是画长一点，这样在游戏中才会显示得比较正常。此外，车子还应避免有太多颜色，不要太过花哨，因为在游戏中的旋转缩放操作会导致车子上面的色彩变质，此时如果车子本来就很斑斓，那么最终的效果是车子身上的颜色稀奇古怪难以辨认。

还有就是尽量以一种从后往前看车的视角来绘制车子，这样在游戏中的立体感会更强。

最好车子不要太大，要自己去测试它能否适应赛道的大小，太大的话很容易一开起来就被卡住，因为在游戏中车子不是视为质点的，而是会对整个车身进行碰撞检测。

## 踩坑

### 踩坑 1

随着整张地图的旋转，小车在地图上的坐标也要转动，一开始我没想到好的解法，也没太多时间（因为是周末），于是我干脆就在一个空白的 IMAGE 对象的小车的位置画一个矩形，
再旋转整个 IMAGE 到地图旋转角，再在 IMAGE 对象上找到矩形的坐标也就对应着小车的坐标。

Ver 2.0 中使用了新的算法直接算出了小车的坐标，速度大为提升。

### 踩坑 2

2D 转 “3D” 的过程需要对 IMAGE 对象逐条线段取样以实现近大远小的透视效果（见 `GetPlayerView` 函数），比较耗时，是游戏卡顿的主要缔造者之一。

### 踩坑 3

小车逆行判定 Bug。原作中的小车逆行判定是这样的：
```
// 注：PForward 小车朝向，Pspeed 小车速度
if (cos(PForward) * Pspeed < 0)
{
  //...
}
```
这段代码的意思是：如果小车的前进方向在一、三象限，那么判定为行驶方向正确，否则就是逆行（乘以 Pspeed 是考虑倒车情况，虽然小车朝前，但是运动方向向后）。

这也就意味着终点线必须是竖直的（即与 y 轴平行），才能满足“小车的前进方向在一、三象限即为行驶方向正确”的要求。

所以如果要支持全朝向的终点线，则需要将代码改成这样：
```
// 注：PForward 小车朝向，EndLineForward 终点线朝向，Pspeed 小车速度
if (cos(PForward - EndLineForward) * Pspeed < 0)
{
  //...
}
```
也就是将坐标系设置为以终点线朝向为 0°，再判定小车的前进方向，这种方法就是正确的了。

一开始我没理解原作者的代码，稀里糊涂写了个新的逆行判定，贴出来让大家一乐：
```
// 得到变量的正负
#define Sgn(x) (x > 0 ? 1 : -1)

// 获取化简弧度
// r 弧度
// min, max 目标弧度范围
double GetSimplifiedRadian(double r, double min, double max)
{
	while (r < min)	r += PI;
	while (r > max)	r -= PI;
	return r;
}

// 玩家是否逆行
// dPlayerForward	玩家朝向
// dLineForward		终点朝向
// dSpeed			玩家速度
bool isRetrograde(double dPlayerForward, double dLineForward, double dSpeed)
{
	// 不被判定为逆行的玩家朝向和终点朝向之差值的阈值
	const double b = PI / 1.5;

	// 若车子前进方向为负，则小车朝向需要 +180°
	double df = GetSimplifiedRadian(dPlayerForward, -PI, PI) + (Sgn(dSpeed) > 0 ? 0 : PI);

	// 若车子朝向与终点线朝向之差大于阈值则判定为逆行
	return abs(df - GetSimplifiedRadian(dLineForward, -PI, PI)) > b;
}
```
这个方法的原理也就是判定小车前进方向与终点线朝向的夹角，如果大于某个阈值就判定为逆行，代码很啰嗦并且效果并不更好。

### 还有吗

还踩了好些坑，不一一列举了。

## 更新日志

**Ver 2.0** (2022.01.22)

1. 新增【双鸭山】地图

2. 优化了小车坐标旋转算法，速度提示 60 倍以上。（详见 `GetRotatedCarPosition` 和 `GetRotatedCarPosition2` 函数）

3. 修复逆行判定机制仅支持竖直终点线的问题

4. 新增超速过线判定机制，见 `CheckPass` 函数。

5. 新增低分辨率模式，以适配小显示器。

6. 新增地图目标等级记录（分三个等级记录在 ini 中）

7. 新增胜利界面

8. 菜单界面更新，且加入 bgm。

9. 地图选择页面可按 ESC 键返回

10. 车速提高时转向速度相应提高

11. 修复不同大小车辆加载异常问题

12. 动态调整玩家视角后视距

13. 补回了上个版本疏漏加入的说明文件，并进行了修改。

14. 对齐地图 b 掩码图中左上方第一个沙坑的位置

15. 由于程序运算耗时长，故新增程序运算时间补偿机制，即将小车位移加以运算耗时权重。

15. 新增文件 NewDrawer.h，将绘图功能模块独立出来。

16. 更正单词拼写（`button` 和 `bottom`、`wide` 和 `width` 等）。

17. 优化诸多细节方面的代码逻辑。

**Ver 1.0** (2021.12.25)

1. 视角改为 2.5D（伪 3D）

2. 新增 SHIFT 键操作

3. 修改了速度等部分参数

4. 显示小地图

5. 显示速度

