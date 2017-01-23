#include "CGL/CGL5.h"


static int run_speed = 3;


using namespace std;
using namespace cgl;


class pencil
{
  public:
	position x, y;
	point nib;
};


class Greedy_snake
{
  protected:
	window win;
	surface pic;
	surface foods;
	  list < pencil > snake;
	int default_x;
	int default_y;
	pencil head, food;
	int up = 0;
	int down = 1;
	int left = 2;
	int right = 3;
	int direction = right;
	  cgl::random ran;
	string die_cause;
	bool check(int x, int y)
	{
	  for (auto it:snake)
			if (x == it.x && y == it.y)
				return false;
		return true;
	}
	void add_food()
	{
		foods.reset();
		int x, y;
		do
		{
			x = ran.get(0, pic.width() - 1);
			y = ran.get(0, pic.high() - 1);
		}
		while (!check(x, y));
		food.x = x;
		food.y = y;
		foods.draw(x, y, food.nib);
	}
	void rst()
	{
		head.x = default_x;
		head.y = default_y;
		snake.clear();
		pic.reset();
		scr.reset();
		direction = right;
		for (int i = 0; i < 3; ++i)
		{
			++head.x;
			snake.push_back(head);
		}
		add_food();
	}
	void die()
	{
		scr.back(color::red);
		scr.reset();
		text txt("You die!");
		txt.back = scr.back();
		txt.x = scr.width() / 2 - txt.size() / 2;
		txt.y = scr.high() / 2;
		scr << txt;
		txt = die_cause;
		txt.x = scr.width() / 2 - txt.size() / 2;
		txt.y = scr.high() / 2 + 1;
		scr << txt;
		txt = "Press any key to continue...";
		txt.x = scr.width() / 2 - txt.size() / 2;
		txt.y = scr.high() - 1;
		scr << txt;
		out << scr;
		scr.back(color::black);
		rst();
		out.GetKbHitPer(run_speed * 100000);
		out.GetKbHit();
	}
	void eat()
	{
		if (direction == up)
			--head.y;
		if (direction == down)
			++head.y;
		if (direction == left)
			--head.x;
		if (direction == right)
			++head.x;
		snake.push_back(head);
		add_food();
	}
	void move_up()
	{
		--head.y;
		snake.push_back(head);
		snake.pop_front();
	}
	void move_down()
	{
		++head.y;
		snake.push_back(head);
		snake.pop_front();
	}
	void move_left()
	{
		--head.x;
		snake.push_back(head);
		snake.pop_front();
	}
	void move_right()
	{
		++head.x;
		snake.push_back(head);
		snake.pop_front();
	}
	void display()
	{
		win.reset();
		pic.reset();
	  for (auto it:snake)
		{
			if (pic.read(it.x, it.y).alpha())
				pic.draw(it.x, it.y, it.nib);
			else
			{
				die_cause = "You eat to yourself.";
				die();
			}
		}
		win.draw(0, 0, pic);
		win.draw(0, 0, foods);
		ControlManager.display(scr, out);
	}
  public:
  Greedy_snake():win(scr)
	{
		AutoWindowAdaptate = false;
		win.show();
		win.focus();
		win.back(color::blue);
		win.title("Greedy Snake");
		pic.set(win.width(), win.high());
		foods.set(win.width(), win.high());
		default_x = 0;
		default_y = pic.high() / 2;
		head.x = default_x;
		head.y = default_y;
		head.nib = '#';
		food.nib = '@';
		head.nib.back = win.back();
		food.nib.back = win.back();
		for (int i = 0; i < 3; ++i)
		{
			++head.x;
			snake.push_back(head);
		}
		add_food();
	}
	void start()
	{
		while (true)
		{
			if (head.x == food.x && head.y == food.y)
				eat();
			if (head.x < 0 || head.x > pic.width() - 1 || head.y < 0 || head.y > pic.high() - 1)
			{
				die_cause = "You hit the wall.";
				die();
			}
			display();
			switch (out.GetKbHitPer(run_speed * 100000))
			{
			case keyboard::num_up:
				if (direction != down)
				{
					direction = up;
					move_up();
				}
				break;
			case keyboard::num_down:
				if (direction != up)
				{
					direction = down;
					move_down();
				}
				break;
			case keyboard::num_select:
				win.title("Greedy Snake-Pause");
				while (out.GetKbHitPer() != keyboard::num_select)
					cgl::ControlManager.display(scr, out);
				win.title("Greedy Snake");
				cgl::ControlManager.display(scr, out);
				break;
			case keyboard::num_left:
				if (direction != right)
				{
					direction = left;
					move_left();
				}
				break;
			case keyboard::num_right:
				if (direction != left)
				{
					direction = right;
					move_right();
				}
				break;
			default:
				if (direction == up)
					move_up();
				if (direction == down)
					move_down();
				if (direction == left)
					move_left();
				if (direction == right)
					move_right();
				break;
			}
		}
	}
};

int start()
{
	Greedy_snake snake;
	snake.start();
}


#include <iostream>


int main()
{
	out.ClrScr();
	cout <<
		"贪吃蛇 CGL版\nCGL版本:" << cgl::cgl_version <<
		"\n操作指南:" << keyboard::num_up << "向上," << keyboard::num_down <<
		"向下," << keyboard::num_left << "向左," << keyboard::
		num_right << "向右," << keyboard::
		num_select << "暂停\n请设置难度系数，低(L)中(M)高(H)或者变态(B)..." <<
		endl;
	switch (out.GetKbHit())
	{
	case 'l':
		run_speed = 3;
		break;
	case 'm':
		run_speed = 2;
		break;
	case 'h':
		run_speed = 1;
		break;
	case 'b':
		run_speed = 0;
		break;
	case 'L':
		run_speed = 3;
		break;
	case 'M':
		run_speed = 2;
		break;
	case 'H':
		run_speed = 1;
		break;
	case 'B':
		run_speed = 0;
		break;
	default:
		run_speed = 2;
		break;
	}
	cout << "按任意键开始。" << endl;
	out.GetKbHit();
	return launch(start);
}