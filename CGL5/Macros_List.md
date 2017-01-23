Covariant Graphics Library
中文:
宏列表
1:CGL_TRACK--打开CGL跟踪模式。必选项会同时打开CGL Debug模式。
提示：这会拖慢运行速度并增加编译时间。
2:CGL_DEBUG--仅打开CGL Debug模式。
提示：这会影响一些运行效率。
3:CGL_KBLAYOUT_PHONE--强制启用CGL手机键盘布局。
4:CGL_KBLAYOUT_PC--强制启用CGL电脑键盘布局。(如果手机键盘布局被启用，这个选项不会起任何作用。你也可以#undef取消定义CGL_KBLAYOUT_PHONE来启用PC键盘布局。)

English:
(Machine translation, there may be some mistakes, please forgive me.)
Macros List
1:CGL_TRACK--Enable CGL Message Track System(Will also enable CGL Debug System)
Tips: This will slow down the running speed and extend the compilation time.
2:CGL_DEBUG--Only enable CGL Debug System
Tips: This will affect running productivity.
3:CGL_KBLAYOUT_PHONE--Mandatory enable CGL Phone Keyboard Layout
4:CGL_KBLAYOUT_PC--Mandatory enable CGL PC Keyboard Layout(If Phone Keyboard Layout has been enabled,this will not have any effect.You can also #undef to cancel the definition CGL_KBLAYOUT_PHONE to enable the PC Keyboard Layout.)