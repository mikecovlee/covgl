#include "CGL/CGL5.h"
#include <sstream>


std::string int2str(int n)
{
	std::stringstream ss;
	std::string str;
	ss << n;
	ss >> str;
	return str;
}


CreateThread(Main)
{
	using namespace cgl;
	window win0(scr.width() * 0.5 - 1, scr.high());
	window win1(scr.width() * 0.5 - 1, scr.high() * 0.5 - 0.5);
	window win2(scr.width() * 0.5 - 1, scr.high() * 0.5 - 0.5);
	win1.x = scr.width() * 0.5 + 1;
	win2.x = win1.x;
	win2.y = scr.high() * 0.5 + 1;
	win0.show();
	win1.show();
	win2.show();
	point pen0, pen1, pen2;
	win0.back(color::blue);
	win1.back(color::green);
	win2.back(color::red);
	win0.title("Marble Window 1");
	win1.title("Marble Window 2");
	win2.title("Marble Window 3");
	auto cls_win =[&]{
		win0.reset();
		win1.reset();
		win2.reset();
	};
	cls_win();
	text txt0, txt1;
	txt0.back = txt1.back = win0.back();
	pen0 = pen1 = pen2 = ' ';
	pen0.back = pen1.back = pen2.back = color::white;
	bool xj0 = true;
	bool xj1 = false;
	bool xj2 = true;
	bool yj0 = true;
	bool yj1 = true;
	bool yj2 = true;
	++txt1.y;
	int x0, x1, x2, y0, y1, y2;
	x0 = x1 = x2 = y0 = y1 = y2 = 0;
	x1 = win1.width() - 1;
	x2 = win2.width() / 2 - 1;
	auto set_txts =[&](int &x, int &y, window & win){
		txt0 = "X=" + int2str(x) + " Width=" + int2str(win.width());
		txt1 = "Y=" + int2str(y) + " High=" + int2str(win.high());
		txt0.back = txt1.back = win.back();
	};
	auto draw_txts =[&](window & win) {
		win << txt0 << txt1;
	};
	SitLoop(Main)
	{
		OnRunningSit
		{
			scr.reset();
			cls_win();
			win0.resize(scr.width() * 0.5 - 1, scr.high());
			win1.resize(scr.width() * 0.5 - 1, scr.high() * 0.5 - 0.5);
			win2.resize(scr.width() * 0.5 - 1, scr.high() * 0.5 - 0.5);
			win1.x = scr.width() * 0.5 + 1;
			win2.x = win1.x;
			win2.y = scr.high() * 0.5 + 1;
			win0.draw(x0, y0, pen0);
			win1.draw(x1, y1, pen1);
			win2.draw(x2, y2, pen2);
			set_txts(x0, y0, win0);
			draw_txts(win0);
			set_txts(x1, y1, win1);
			draw_txts(win1);
			set_txts(x2, y2, win2);
			draw_txts(win2);
			if (x0 <= 0)
				xj0 = true;
			if (y0 <= 0)
				yj0 = true;
			if (x0 >= win0.width() - 1)
				xj0 = false;
			if (y0 >= win0.high() - 1)
				yj0 = false;
			if (x1 <= 0)
				xj1 = true;
			if (y1 <= 0)
				yj1 = true;
			if (x1 >= win1.width() - 1)
				xj1 = false;
			if (y1 >= win1.high() - 1)
				yj1 = false;
			if (x2 <= 0)
				xj2 = true;
			if (y2 <= 0)
				yj2 = true;
			if (x2 >= win2.width() - 1)
				xj2 = false;
			if (y2 >= win2.high() - 1)
				yj2 = false;
			if (xj0)
				++x0;
			else
				--x0;
			if (yj0)
				++y0;
			else
				--y0;
			if (xj1)
				++x1;
			else
				--x1;
			if (yj1)
				++y1;
			else
				--y1;
			if (xj2)
				++x2;
			else
				--x2;
			if (yj2)
				++y2;
			else
				--y2;
			ControlManager.display(scr, out);
			EndSit;
		}
		OnPauseSit
		{
			ThreadIdle;
			EndSit;
		}
		EndSitLoop(Main);
	}
	EndThread(Main, ThreadSuccess);
}


int cgl_main()
{
	Thread(Main).detach();
	WaitForLeisure(Main);
	Thread(Main).resume();
	while (true)
		ThreadIdle;
}


int main()
{
	return cgl::launch(cgl_main);
}