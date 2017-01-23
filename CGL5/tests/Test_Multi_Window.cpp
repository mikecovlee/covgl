#define CGL_DEBUG
#include "CGL/CGL5.h"


int start()
{
	using namespace cgl;
	scr.back(color::white);
	window win0(30, 10), win1(30, 10), win2(30, 10);
	win0.x = win0.y = 10;
	win1.x = win1.y = 5;
	win0.title("Window 1");
	win1.title("Window 2");
	win2.title("Windiw 3");
	win0.back(color::blue);
	win1.back(color::red);
	win2.back(color::green);
	win0.show();
	win1.show();
	win2.show();
	win2.focus();
	win1.focus();
	win0.focus();
	while (true)
	{
		scr.reset();
		ControlManager.display(scr, out);
		switch (out.GetKbHitPer())
		{
		case 'a':
			win0.select();
			break;
		case 'b':
			win1.select();
			break;
		case 'c':
			win2.select();
			break;
		case 'A':
			win0.focus();
			break;
		case 'B':
			win1.focus();
			break;
		case 'C':
			win2.focus();
			break;
		case keyboard::num_up:
			--ControlManager.selected()->y;
			break;
		case keyboard::num_down:
			++ControlManager.selected()->y;
			break;
		case keyboard::num_left:
			--ControlManager.selected()->x;
			break;
		case keyboard::num_right:
			++ControlManager.selected()->x;
			break;
		case '*':
			ControlManager.selected()->show();
			break;
		case '#':
			ControlManager.selected()->hide();
			break;
		}
	}
	return 0;
}


int main()
{
	return cgl::launch(start);
}