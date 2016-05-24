#include "CGL/CGL5.h"

int start()
{
	using namespace std;
	using namespace cgl;
	screen scr;
	surface pic;
	pic.set(scr.width(), scr.high());
	point pen;
	pen.back = color::white;
	pen.text = color::black;
	pen = ' ';
	cgl::random ran;
	int x, y;
	while (true)
	{
		pic.set(scr.width(), scr.high());
		x = ran.get(0, scr.width() - 1);
		y = ran.get(0, scr.high() - 1);
		if (out.GetKbHitPer() != -1)
			pic.reset();
		if (!pic.read(x, y).alpha())
			continue;
		pic.draw(x, y, pen);
		scr.reset();
		scr.draw(0, 0, pic);
		out << scr;
	}
	cgl::clock.delay(cgl::time::min, 5);
}

int main()
{
	return cgl::launch(start);
}