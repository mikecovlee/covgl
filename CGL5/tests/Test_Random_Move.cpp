#include "CGL/CGL5.h"

int start()
{
	using namespace std;
	using namespace cgl;
	screen scr;
	scr.back(color::white);
	point pen;
	pen.back = scr.back();
	pen.text = color::black;
	pen = '@';
	int x, y;
	x = scr.width() / 2;
	y = scr.high() / 2;
	scr.draw(x, y, pen);
	cgl::random ran;
	while (true)
	{
	  repeat:
		switch (ran.get(1, 9))
		{
		case 1:
			{
				if (x > 0)
					--x;
				else
					++x;
				if (y > 0)
					--y;
				else
					++y;
				break;
			}
		case 2:
			{
				if (y > 0)
					--y;
				else
					++y;
				break;
			}
		case 3:
			{
				if (x < scr.width() - 1)
					++x;
				else
					--x;
				if (y > 0)
					--y;
				else
					++y;
				break;
			}
		case 4:
			{
				if (x > 0)
					--x;
				else
					++x;
				break;
			}
		default:
			{
				goto repeat;
			}
		case 6:
			{
				if (x < scr.width() - 1)
					++x;
				else
					--x;
				break;
			}
		case 7:
			{
				if (x > 0)
					--x;
				else
					++x;
				if (y < scr.high() - 1)
					++y;
				else
					--y;
				break;
			}
		case 8:
			{
				if (y < scr.high() - 1)
					++y;
				else
					--y;
				break;
			}
		case 9:
			{
				if (x < scr.width() - 1)
					++x;
				else
					--x;
				if (y < scr.high() - 1)
					++y;
				else
					--y;
				break;
			}
		}
		scr.reset();
		scr.draw(x, y, pen);
		out << scr;
		if (out.GetKbHitPer() != -1)
			break;
	}
	cgl::clock.delay(cgl::time::min, 5);
}

int main()
{
	return cgl::launch(start);
}