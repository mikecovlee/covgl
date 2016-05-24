#include "CGL/CGL5.h"
#include <fstream>
#include <thread>


using namespace cgl;
using namespace std;


static bool hang_up = true;
static bool running = true;
static char input;


void main_func()
{
	while (running)
	{
		if (!hang_up)
		{
			input = out.GetKbHit();
			hang_up = true;
		}
	}
}


class text_editor
{
  protected:
	string tmp0;
	string tmp1;
	string tmp2;
	  vector < string > buff;
	int x, y;
	int sx, sy;
	bool draw_underline_special = false;
	window win, menu;
	text txt0, txt1, txt2;
	surface signs, render;
	thread igt;
	void new_char(string &, int, char);
	void del_char(string &, int);
	void new_line(vector < string > &, int, const string &);
	void del_line(vector < string > &, int);
	const string & truncate_line(string &, int);
	void set_txts(text &, text &, text &);
	void draw_underline();
	void draw_text();
	void draw_all();
	int run_menu();
  public:
	enum sign
	{
		create_new_file = 0x0a,
		open_old_file = 0x0b,
		exit_with_save = 0x0c,
		exit_without_save = 0x0d
	};
	int modle = sign::create_new_file;
	  text_editor();
	int run();
	bool open_file(const char *);
	bool save_file(const char *);
};


void text_editor::new_char(string & str, int posit, char ch = ' ')
{
	tmp2 = "";
	for (int i = 0; i <= posit && i < str.size(); ++i)
		tmp2.push_back(str[i]);
	tmp2.push_back(ch);
	for (int i = posit + 1; i < str.size(); ++i)
		tmp2.push_back(str[i]);
	str = tmp2;
}


void text_editor::del_char(string & str, int posit)
{
	tmp2 = "";
	for (int i = 0; i < posit; ++i)
		tmp2.push_back(str[i]);
	for (int i = posit + 1; i < str.size(); ++i)
		tmp2.push_back(str[i]);
	str = tmp2;
}


void text_editor::new_line(vector < string > &vec, int line, const string & str = "")
{
	if (line == vec.size() - 1)
		vec.push_back(str);
	else
	{
		vector < string > tmp;
		tmp.resize(vec.size() + 1);
		for (int i = 0; i <= line; ++i)
			tmp[i] = vec[i];
		tmp[line + 1] = str;
		for (int i = line + 2; i < tmp.size(); ++i)
			tmp[i] = vec[i - 1];
		vec = tmp;
	}
}

void text_editor::del_line(vector < string > &vec, int line)
{
	if (line == vec.size() - 1)
		vec.pop_back();
	else
	{
		vector < string > tmp;
		tmp.resize(vec.size() - 1);
		for (int i = 0; i < line; ++i)
			tmp[i] = vec[i];
		for (int i = line; i < tmp.size(); ++i)
			tmp[i] = vec[i + 1];
		vec = tmp;
	}
}


const string & text_editor::truncate_line(string & buf, int posit)
{
	tmp0 = "";
	tmp1 = "";
	for (int i = 0; i < posit; ++i)
		tmp0.push_back(buf[i]);
	for (int i = posit; i < buf.size(); ++i)
		tmp1.push_back(buf[i]);
	buf = tmp0;
	return tmp1;
}


void text_editor::set_txts(text & t0, text & t1, text & t2)
{
	t0.text = menu.back();
	t0.back = menu.border_color();
	t1.text = menu.border_color();
	t1.back = menu.back();
	t2.text = menu.border_color();
	t2.back = menu.back();
}


void text_editor::draw_underline()
{
	if (sx < 0 && x > 0)
	{
		int v;
		if (x + sx < 0)
			v = 0;
		else
			v = x + sx;
		if (draw_underline_special)
			v = x;
		point tmp = render.read(v, y);
		if (tmp != ' ')
			tmp.attribute(attribute::underline);
		else
			tmp = '_';
		render.draw(v, y, tmp);
	}
	else
		render.draw(x, y, '_');
}


void text_editor::draw_text()
{
	render.reset();
	if (buff.size() > render.high())
	{
		y = render.high() - 1;
		int s = 0;
		if (sy < 0)
		{
			if (-sy < buff.size() - render.high())
				s = -sy;
			else
				s = buff.size() - render.high();
		}
		int i = buff.size() - 1 - s;
		while (y >= 0 && i >= 0)
		{
			draw_underline_special = false;
			if (i == buff.size() - 1 + sy)
			{
				if (buff[i].size() + sx > render.width() - 2)
				{
					int v = buff[i].size() - 1 + sx;
					if (sx < 0)
					{
						++v;
						for (int k = render.width() - 1; k >= 0; --k, --v)
							render.draw(k, y, buff[i][v]);
					}
					else
					{
						for (int k = render.width() - 2; k >= 0; --k, --v)
							render.draw(k, y, buff[i][v]);
					}
					x = render.width() - 1;
					draw_underline_special = true;
				}
				else
				{
					for (x = 0; x < buff[i].size(); ++x)
						render.draw(x, y, buff[i][x]);
				}
			}
			else
				for (x = 0; x < buff[i].size(); ++x)
					render.draw(x, y, buff[i][x]);
			if (i > render.high() - 1)
			{
				if (y == render.high() - 1)
					draw_underline();
			}
			else if (y == render.high() - 1 + buff.size() - render.high() + sy)
				draw_underline();
			--y;
			--i;
		}
		y = render.high();
	}
	else
	{
		for (y = 0; y < buff.size(); ++y)
		{
			draw_underline_special = false;
			if (y == buff.size() - 1 + sy)
			{
				if (buff[y].size() + sx > render.width() - 2)
				{
					int v = buff[y].size() - 1 + sx;
					if (sx < 0)
					{
						++v;
						for (int k = render.width() - 1; k >= 0; --k, --v)
							render.draw(k, y, buff[y][v]);
					}
					else
					{
						for (int k = render.width() - 2; k >= 0; --k, --v)
							render.draw(k, y, buff[y][v]);
					}
					x = render.width() - 1;
					draw_underline_special = true;
				}
				else
				{
					for (x = 0; x < buff[y].size(); ++x)
						render.draw(x, y, buff[y][x]);
				}
			}
			else
				for (x = 0; x < buff[y].size(); ++x)
					render.draw(x, y, buff[y][x]);
			if (y == buff.size() - 1 + sy)
				draw_underline();
		}
	}
}


void text_editor::draw_all()
{
	scr.reset();
	win.reset();
	render.reset();
	signs.reset();
	render.set(win.width() - 2, win.high());
	signs.set(2, win.high());
	draw_text();
	for (int i = 0; i < y && i < signs.high(); ++i)
	{
		signs.draw(0, i, '/');
		signs.draw(1, i, '>');
	}
	win.draw(0, 0, signs);
	win.draw(2, 0, render);
	ControlManager.display(scr, out);
}


int text_editor::run_menu()
{
	fflush(stdin);
	menu.show();
	menu.focus();
	txt0.show();
	txt1.show();
	txt2.show();
	txt0.focus();
	txt1.focus();
	txt2.focus();
	txt0.select();
	menu.resize(scr.width() / 2, scr.high() / 2);
	menu.x = scr.width() / 2 - menu.width(true) / 2;
	menu.y = scr.high() / 2 - menu.high(true) / 2;
	txt0.x = scr.width() / 2 - txt0.size() / 2;
	txt1.x = scr.width() / 2 - txt1.size() / 2;
	txt2.x = scr.width() / 2 - txt2.size() / 2;
	txt0.y = scr.high() / 2 - 1;
	txt1.y = scr.high() / 2;
	txt2.y = scr.high() / 2 + 1;
	draw_all();
	bool lastctrl = false;
	bool selectctrl = false;
	bool nextctrl = false;
	bool work = true;
	int ret = 0;
	while (work)
	{
		if (hang_up)
		{
			switch (input)
			{
			case keyboard::num_up:
				lastctrl = true;
				break;
			case keyboard::num_select:
				selectctrl = true;
				break;
			case keyboard::num_down:
				nextctrl = true;
				break;
			}
			if (lastctrl)
			{
				if (txt0.selected())
					txt2.select();
				else if (txt1.selected())
					txt0.select();
				else if (txt2.selected())
					txt1.select();
				lastctrl = false;
			}
			if (nextctrl)
			{
				if (txt0.selected())
					txt1.select();
				else if (txt1.selected())
					txt2.select();
				else if (txt2.selected())
					txt0.select();
				nextctrl = false;
			}
			if (selectctrl)
			{
				if (txt1.selected())
					ret = sign::exit_without_save;
				if (txt2.selected())
					ret = sign::exit_with_save;
				hang_up = true;
				work = false;
				selectctrl = false;
			}
			if (work)
				hang_up = false;
			if (txt0.selected())
				set_txts(txt0, txt1, txt2);
			if (txt1.selected())
				set_txts(txt1, txt0, txt2);
			if (txt2.selected())
				set_txts(txt2, txt1, txt0);
		}
		menu.resize(scr.width() / 2, scr.high() / 2);
		menu.x = scr.width() / 2 - menu.width(true) / 2;
		menu.y = scr.high() / 2 - menu.high(true) / 2;
		txt0.x = scr.width() / 2 - txt0.size() / 2;
		txt1.x = scr.width() / 2 - txt1.size() / 2;
		txt2.x = scr.width() / 2 - txt2.size() / 2;
		txt0.y = scr.high() / 2 - 1;
		txt1.y = scr.high() / 2;
		txt2.y = scr.high() / 2 + 1;
		draw_all();
	}
	menu.hide();
	txt0.hide();
	txt1.hide();
	txt2.hide();
	win.focus();
	draw_all();
	return ret;
}


text_editor::text_editor():x(0), y(0), sx(0), sy(0), win(scr), menu(scr.width() / 2, scr.high() / 2),
igt(main_func)
{
	fflush(stdin);
	win.title("Text Editor");
	win.back(color::blue);
	win.show();
	win.focus();
	menu.title("Menu");
	menu.back(color::white);
	menu.border_color(color::black);
	menu.x = scr.width() / 2 - menu.width(true) / 2;
	menu.y = scr.high() / 2 - menu.high(true) / 2;
	menu.hide();
	txt0.hide();
	txt1.hide();
	txt2.hide();
	set_txts(txt0, txt1, txt2);
	txt0 = "Input character\'$\'";
	txt1 = "Exit without save";
	txt2 = "Save to file and exit";
	txt0.x = scr.width() / 2 - txt0.size() / 2;
	txt1.x = scr.width() / 2 - txt1.size() / 2;
	txt2.x = scr.width() / 2 - txt2.size() / 2;
	txt0.y = scr.high() / 2 - 1;
	txt1.y = scr.high() / 2;
	txt2.y = scr.high() / 2 + 1;
	render.alpha(false);
	signs.alpha(false);
	render.set(win.width() - 2, win.high());
	signs.set(2, win.high());
	signs.back(color::magenta);
}


int text_editor::run()
{
	if (buff.size() == 0)
		buff.push_back("");
	running = true;
	hang_up = false;
	igt.detach();
	int direct_ctrl = 0;
	while (true)
	{
		draw_all();
		if (hang_up)
		{
			char inp = input;
			if (inp == '$')
			{
				int ret = run_menu();
				switch (ret)
				{
				case sign::exit_without_save:
					running = false;
					return sign::exit_without_save;
				case sign::exit_with_save:
					running = false;
					return sign::exit_with_save;
				}
				fflush(stdin);
			}
			hang_up = false;
			string & buf = buff[buff.size() - 1 + sy];
			int bufsiz = buf.size();
			switch (direct_ctrl)
			{
			case 1:
				if (inp == '[')
					direct_ctrl = 2;
				else
					direct_ctrl = 0;
				continue;
			case 2:
				{
					switch (inp)
					{
					case 'A':
						if (-sy < buff.size() - 1)
							--sy;
						else
							sy = 0;
						break;
					case 'B':
						if (sy < 0)
							++sy;
						else
							sy = -(buff.size() - 1);
						break;
					case 'C':
						if (sx < 0)
							++sx;
						break;
					case 'D':
						if (bufsiz + sx > 0)
							--sx;
						break;
					}
					if (buff[buff.size() - 1 + sy].size() < -sx)
						sx = 0;
					direct_ctrl = 0;
					continue;
				}
			}
			if (inp == keyboard::enter)
			{
				if (sx < 0)
				{
					if (bufsiz - 1 + sx < 0)
					{
						new_line(buff, buff.size() - 1 + sy, buf);
						buff[buff.size() - 2 + sy] = "";
					}
					else
					{
						new_line(buff, buff.size() - 1 + sy, truncate_line(buf, bufsiz + sx));
					}
				}
				else
					new_line(buff, buff.size() - 1 + sy);
				continue;
			}
			if (inp == 27)
			{
				direct_ctrl = 1;
				continue;
			}
			if (inp == keyboard::back_space)
			{
				if (buff.size() > 0)
				{
					if (bufsiz > 0)
					{
						if (sx < 0)
						{
							if (bufsiz + sx <= 0)
							{
								if (buff.size() > 1 && buff.size() - 1 + sy > 0)
								{
									buff[buff.size() - 2 + sy] += buf;
									del_line(buff, buff.size() - 1 + sy);
								}
							}
							else
							{
								int v = bufsiz - 1 + sx;
								del_char(buf, v);
							}
						}
						else
							buf.pop_back();
					}
					else
					{
						if (buff.size() > 1 && buff.size() - 1 + sy > 0)
							del_line(buff, buff.size() - 1 + sy);
					}
					continue;
				}
			}
			if (sx < 0)
			{
				int v;
				if (bufsiz - 1 + sx < 0)
					v = -1;
				else
					v = bufsiz - 1 + sx;
				new_char(buf, v, inp);
			}
			else
				buf.push_back(inp);
		}
	}
	running = false;
	return sign::exit_without_save;
}


bool text_editor::open_file(const char *place)
{
	ifstream in(place);
	if (in)
	{
		std::string temp;
		while (getline(in, temp))
			buff.push_back(temp);
		modle = sign::open_old_file;
		return true;
	}
	else
		return false;
}


bool text_editor::save_file(const char *place)
{
	ofstream out(place);
	if (out)
	{
		for (int i = 0; i != buff.size(); ++i)
			out << buff[i] << endl;
		return true;
	}
	else
		return false;
}


text_editor txtEdit;


int start()
{
	return txtEdit.run();
}


#include <iostream>


int main()
{
	using namespace std;
	cgl::out.ClrScr();
	cout << "Welcome to use CGL Text Editor.\n"
		"Please make a choice:\n" "1:New File(default)\n" "2:Open File" << endl;
	char in = cgl::out.GetKbHit();
	if (in == '2')
	{
		cout << "Please input the file directory." << endl;
		string place;
		getline(cin, place);
		cout << "Please enter the file name:" << endl;
		string filename;
		getline(cin, filename);
		place += "/" + filename;
		txtEdit.open_file(place.c_str());
	}
	int ret;
	cgl::launch(ret, start);
	if (ret == txtEdit.sign::exit_with_save)
	{
		cout << "Please input you want to save the file directory." << endl;
		string place;
		getline(cin, place);
		cout << "Please enter a file name:" << endl;
		string filename;
		getline(cin, filename);
		place += "/" + filename;
		if (txtEdit.save_file(place.c_str()))
		{
			cout << "Save the file successfully.";
			return 0;
		}
		else
		{
			cout << "Save the file failed.File direction is " << place;
			return 1;
		}
	}
	cout << "CGL Text Editor has been exited." << endl;
	return 0;
}