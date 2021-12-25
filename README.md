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

## 新增内容

1. 视角改为 2.5D（伪 3D）

2. 新增 SHIFT 键操作

3. 修改了速度等部分参数

4. 显示小地图

5. 显示速度

## 想法

作为一名忙碌的学生党……在周六回家的时候挤了一点时间做了这个修改版。
我感觉原作做的其实很好，但是我玩着玩着就觉得这个游戏和我们一般玩的赛车游戏有点点不对味，
因为我们玩的赛车游戏一般都是第一人称的，不会看到整张地图来开车……
所以我就心血来潮做了这个修改版。

我通过跟随游戏中小车的视角，把第一人称的视角在地图中获取下来，并通过“近大远小”的原理，
模拟出 3D 的感觉，所以这是个伪 3D 游戏，我就把它称作“2.5D”了。

跟随小车的视角，其实只需要把小车绘制到地图上，再把整个地图转到和小车相同的角度，这样小车所朝方向就是正方向了，
然后再从旋转后的地图上，从小车的位置开始，往前获取一段图像就可以了（当然还要做“近大远小”的透视效果）。

原理其实很简单，但是真正上手做的时候有一个需要注意的地方，也是我踩坑的地方，也就是随着整张地图的旋转，小车在地图上的坐标也要转动，
获取小车的坐标看起来并不难，然而地图是个 IMAGE 对象，使用 rotateimage 方法转动它时，IMAGE 对象的长宽会发生变化，
这样的话小车的坐标就再需要经过一些运算才能得出正确结果……我这样描述可能不能让你完全理解我在说什么，但如果你亲自去尝试实现一下就会明白的。
在这种情况下，小车的坐标算是能算，不过我一下还写不出来（也没那么多时间），所以我干脆就用了一个简单粗暴的办法，
也就是再用一个空白的 IMAGE 对象，上面画一个小矩形，矩形的位置也就是小车的位置，再旋转整个 IMAGE 到对应的角度，
这个角度和地图旋转的角度相同，这样我再在 IMAGE 对象上找到矩形的坐标，就知道小车的坐标了。虽然笨笨的，但是总算是实现了。

由于 2D 转 “3D” 的过程比较耗时，我的能力也暂时无法对其进行太大的优化，所以游戏会卡顿一些……不过还是很好玩的。

也可以手动修改 Draw3D_PlayerView 函数中的各项视野参数，如果调小一点的话会流畅一些。

嗯……玩的开心吧！
