#include "CGL/CGL5.h"


int start()
{
	cgl::text txt;
	int x, y;
	x = y = 2;
	x *= 2;
	int bc = 10;
	while (true)
	{
		cgl::screen scr;
		cgl::surface surf;
		surf.set(scr.width(), scr.high());
		txt = "Test!!";
		scr.back(cgl::color::blue);
		surf.back(cgl::color::blue);
		if (txt.onTextChange.demand())
		{
			txt.back = scr.back();
			scr << txt;
		}
		for (int i = 0; i <= bc; ++i)
		{
			surf.draw(x, y + i, '|');
			surf.draw(x + bc * 2, y + i, '|');
		}
		for (int i = 0; i <= bc * 2; ++i)
		{
			surf.draw(x + i, y, '-');
			surf.draw(x + i, y + bc, '-');
		}
		surf.draw(x, y, '+');
		surf.draw(x, y + bc, '+');
		surf.draw(x + 2 * bc, y, '+');
		surf.draw(x + 2 * bc, y + bc, '+');
		scr.draw(0, 0, surf);
		cgl::out << scr;
		switch (cgl::out.GetKbHitPer())
		{
		case cgl::keyboard::num_up:
			--y;
			break;
		case cgl::keyboard::num_down:
			++y;
			break;
		case cgl::keyboard::num_left:
			--x;
			break;
		case cgl::keyboard::num_right:
			++x;
			break;
		}
	}
	return 0;
}


int main()
{
	return cgl::launch(start);
}