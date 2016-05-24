#include "CGL/CGL5.h"
int main()
{
	using namespace cgl;		// 使用cgl名称空间
	auto run =[&]{				// 主Lambda函数
		scr.back(color::white);	// 设置屏幕背景色为白色
		text txt("A");			// 新建文本，内容为"A"
		txt.text = color::black;	// 设置文本色为黑色
		txt.back = scr.back();	// 设置背景色与屏幕背景色相同
		txt.show();				// 显示文本
		while (out.GetKbHitPer(500000) != ' ')	// 在500000毫秒内获取键盘输入，判断是否为空格
		{
			txt.x = scr.width() / 2 - txt.size() / 2;	// 设置x坐标为屏幕中心
			txt.y = scr.high() / 2;	// 设置y坐标为屏幕中心
			scr.reset();		// 重置屏幕
			ControlManager.display(scr, out);	// 绘制所有控件并在out输出流上显示当前屏幕
			if (txt.visible())	// 如果当然文本可见就隐藏，反之显示
				txt.hide();
			else
				txt.show();
		}
		return 0;
	};
	launch(run);				// 启动run函数
	return 0;
}