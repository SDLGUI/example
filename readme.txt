本例子可以在WIN下编译，不能在手机平台编译。
编译前先将SDLGUI所要用的库文件编译环境设置好
SDLGUI所需库有SDL2,SDL2_TTF,SDL2_IMAGE,SDL2_gfx,SDL2_net,SDL2_mixer


以下连接是SDLGUI库所用的库文件包。
http://pan.baidu.com/s/1hqmkhXa


本例子在mingw32 GCC下编译通过
在终端输入以下命令行并回车
make或make test.exe

如果使用可视编辑软件(如codeblack)要设置好头文件路径和库文件路径
库文件加载顺序为
-lSDL2_gfx
-lSDL2_image
-lSDL2_image.dll
-lSDL2_draw
-lSDL2_ttf
-lSDL2_ttf.dll
-lSDL2_mixer
-lSDL2_mixer.dll
-lSDL2_net
-lSDL2_net.dll
-lmingw32
-lSDL2main
-lSDL2
-lSDL2.dll










本例子的游戏为《打方块》


游戏代码结构

main.cpp	游戏主进程源码

hitbox.html	游戏展示视频

core文件夹	游戏内核算法，实现方块的填充与摘取，并进行游戏信息的更新与状态反馈.

game_gui文件夹	游戏GUI层的显示与输入输出

sdlgui文件夹	SDLGUI库文件，实现GUI底层实现与窗口管理。

bin文件夹	游戏程序文件，运行test.exe开始游戏



游戏使用方法

每过一关后点击开始进入下一关
如果本关还没通过时点击开始则重新再玩本关
使用键盘上的【四个方向键】移动游戏中的活动方块（即在闪动的方块），
每关有【3个可用方块】，使用键盘上的【四个方向键】把它们打到所有【深色方块】中则【过关】
【最亮的方块】和【填充后的方块】均为【障碍物】，填充时只能把方块打到障碍物的四周。








本例子的目标
1、解决上个版本出现的BUG：不能编译与运行。
2、用于测试SDLGUI的可行性，发现在绘图方面非常方便，所用即所见的效果。但在性能上还有很大的改进空间，主要表现在事件处理上。
3、用于测试SDLGUI在游戏开发上的适用性，个人感觉非常好，这个游戏分为两层，第一层为游戏计算核心层，第二层为游戏人机交流表现层。SDLGUI在这两层上可以很好的交换数据。
本例子的BUG
在填充方块时，如果在可行走的方块旁边进行填充时会出错，但不影响游戏运行。是由于游戏内核算法引起的。