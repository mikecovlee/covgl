#include "CGL/CGL5.h"


void synt(cgl::surface & picdice, int dice[])
{
	picdice.reset();
	picdice.back(cgl::color::white);
	cgl::point pen;
	pen = '@';
	pen.text = cgl::color::black;
	pen.back = cgl::color::white;
	for (int k = 0; k < 25; ++k)
	{
		if (dice[k] == 1)
		{
			int i = k + 1;
			int x, y;
			if (i % 5 == 0)
			{
				x = 5 * 4 - 4;
				y = (i / 5) * 2 - 2;
			}
			else
			{
				x = (i % 5) * 4 - 4;
				y = (((i - i % 5) / 5) + 1) * 2 - 2;
			}
			picdice.draw(x, y, pen);
		}
	}
}


int start()
{
	int dice1[] = {
		0, 0, 0, 0, 0,
		0, 0, 0, 0, 0,
		0, 0, 1, 0, 0,
		0, 0, 0, 0, 0,
		0, 0, 0, 0, 0
	};
	int dice2[] = {
		0, 0, 0, 0, 0,
		0, 0, 0, 0, 0,
		0, 1, 0, 1, 0,
		0, 0, 0, 0, 0,
		0, 0, 0, 0, 0
	};
	int dice3[] = {
		0, 0, 0, 0, 0,
		0, 0, 1, 0, 0,
		0, 0, 0, 0, 0,
		0, 1, 0, 1, 0,
		0, 0, 0, 0, 0
	};
	int dice4[] = {
		0, 0, 0, 0, 0,
		0, 1, 0, 1, 0,
		0, 0, 0, 0, 0,
		0, 1, 0, 1, 0,
		0, 0, 0, 0, 0
	};
	int dice5[] = {
		0, 0, 0, 0, 0,
		0, 1, 0, 1, 0,
		0, 0, 1, 0, 0,
		0, 1, 0, 1, 0,
		0, 0, 0, 0, 0
	};
	int dice6[] = {
		0, 0, 0, 0, 0,
		0, 1, 1, 1, 0,
		0, 0, 0, 0, 0,
		0, 1, 1, 1, 0,
		0, 0, 0, 0, 0
	};
	cgl::scr.back(cgl::color::blue);
	cgl::random ran;
	cgl::surface dice;
	dice.set(17, 9);
	dice.alpha(false);
	cgl::text txt("Dice Simulator by Mike Lee");
	txt.back = cgl::scr.back();
	auto applytxt =[&]{
		txt.x = cgl::scr.width() / 2 - txt.size() / 2;
		txt.y = 1;
		cgl::scr.reset();
		cgl::scr << txt;
		cgl::out << cgl::scr;
	};
	auto applydice =[&]{
		txt.x = cgl::scr.width() / 2 - txt.size() / 2;
		txt.y = 1;
		cgl::scr.reset();
		cgl::scr << txt;
		cgl::scr.draw(cgl::scr.width() / 2 - dice.width() / 2,
					  cgl::scr.high() / 2 - dice.high() / 2, dice);
		cgl::out << cgl::scr;
	};
	while (true)
	{
		cgl::clock.delay(cgl::time::sec, 1, applytxt);
		switch (ran.get(1, 6))
		{
		case 1:
			synt(dice, dice1);
			break;
		case 2:
			synt(dice, dice2);
			break;
		case 3:
			synt(dice, dice3);
			break;
		case 4:
			synt(dice, dice4);
			break;
		case 5:
			synt(dice, dice5);
			break;
		case 6:
			synt(dice, dice6);
			break;
		}
		cgl::clock.delay(cgl::time::sec, 3, applydice);
	}
	return 0;
}


int main()
{
	return cgl::launch(start);
}