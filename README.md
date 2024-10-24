# 植物大战僵尸双人版

## 介绍

本程序是基于 EasyX Graphics Library 开发的 C++ 双人游戏，游戏沿用了原版植物大战僵尸中僵尸博士复仇的风格，但还有很多未实现的功能，用户可以通过鼠标或键盘控制植物的摆放，僵尸的摆放。

## 游戏说明

### 植物

植物的操作和原版一致，通过鼠标左键控制植物的摆放，和原版的区别是摆放时没有幽灵提示（ghost hint），摆放时可能会放错位置。

**植物类型：**

| 属性 | 卷心菜 | 西瓜 | 玉米 | 寒冰菇 | 辣椒 | 花盆 |
|:-:|:-:|:-:|:-:|:-:|:-:|:-:|
| 血量 | 6 | 6 | $\infty$ | $\infty$ | $\infty$ | 6 |
| 攻击 | 40 | 80 | 1800 | 100 | 1800 | 0 |
| 特殊说明 | 无 | 无 | 灰烬植物，改为一颗玉米加农炮炮弹，但只能放置在花盆上 | 无 | 无 | 无 |

### 僵尸

僵尸通过键盘放置，上下键可以控制僵尸应该放在哪一行。

**僵尸类型：**

| 属性 | 普通僵尸 | 路障僵尸 | 铁通僵尸 | 橄榄球僵尸 | 投石车僵尸 | 小丑僵尸 |
|:-:|:-:|:-:|:-:|:-:|:-:|:-:|
| 放置键 | z/Z | x/X | c/C | v/V | b/B | n/N |
| 血量 | 200 | 400 | 1000 | 1800 | 1200 | 400 |
| 攻击 | 1 | 1 | 1 | 1 | 2 | $\infty$ |
| 行走速度 | 1 | 1 | 1 | 2 | 0 | 1 |
| 特殊说明 | 无 | 无 | 无 | 无 | 只会攻击你最后一排的植物，它不会向前移动，也不会压碎植物 | 死亡后爆炸，摧毁小丑面前的三株靠近的植物 |

## 可能存在的问题

Q：为什么把植物拖到非法位置（比如天空，月亮，最后一排后面）点击就消失了？

A：因为程序在处理卡牌的时候没有做撤消操作，也就是只要把卡牌拿出来就只能​种上，所以可以点击种植区外的任意位置来破坏植物，这也避免一种极端情况：花盆上种满植物，但是手里的植物却不能种，放回去或者破坏）。

Q：为什么第九排放不了植物？

A：第九排是放僵尸的，不允许放置植物。

Q：为什么有时候玉米加农炮打不到投石车僵尸？

A：投石车僵尸体型过大，但判定和其他僵尸是一样的，判定在僵尸的后半身。

## 致谢

**感谢 [Ooil9965](https://github.com/Ooil9965) 提供的素材! ! !**

## 更新

本次更新修改了程序的结构，从 visual studio sln 模式改成了 cmake 模式，增加了一些模块化的处理，将渲染函数和游戏处理函数进行了异步化，提高游戏的流畅性（好像本来也不卡）。