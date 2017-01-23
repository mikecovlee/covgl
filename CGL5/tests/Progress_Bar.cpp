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


using namespace cgl;


int cgl_main()
{
	window win(scr);
	win.title("Waiting...");
	win.back(color::blue);
	scr.reset();
	win.reset();
	scr << win;
	out << scr;
	std::string proc(win.width() - 2, '=');
	std::string pers(int2str(0) + "%");
	text process(proc);
	text persent(pers);
	process.x = 1;
	process.y = 1;
	process.back = color::green;
	persent.back = color::blue;
	persent.x = win.width() / 2 - persent.size() / 2;
	persent.y = 2;
	win << process;
	scr << win;
	out << scr;
	while (true)
	{
		for (int i = 0; i != proc.size(); ++i)
		{
			scr.reset();
			win.reset();
			proc[i] = '>';
			process = proc;
			double proce = (i * 100) / proc.size();
			persent = int2str(proce) + "%";
			persent.x = win.width() / 2 - persent.size() / 2;
			win << process << persent;
			scr << win;
			out << scr;
		}
		for (int i = proc.size(); i != 0; --i)
		{
			scr.reset();
			win.reset();
			proc[i - 1] = '=';
			process = proc;
			double proce = (i * 100) / proc.size();
			persent = int2str(proce) + "%";
			persent.x = win.width() / 2 - persent.size() / 2;
			win << process << persent;
			scr << win;
			out << scr;
		}
		for (int i = proc.size(); i != 0; --i)
		{
			scr.reset();
			win.reset();
			proc[i - 1] = '<';
			process = proc;
			double proce = (i * 100) / proc.size();
			persent = int2str(proce) + "%";
			persent.x = win.width() / 2 - persent.size() / 2;
			win << process << persent;
			scr << win;
			out << scr;
		}
		for (int i = 0; i != proc.size(); ++i)
		{
			scr.reset();
			win.reset();
			proc[i] = '=';
			process = proc;
			double proce = (i * 100) / proc.size();
			persent = int2str(proce) + "%";
			persent.x = win.width() / 2 - persent.size() / 2;
			win << process << persent;
			scr << win;
			out << scr;
		}
	}
}


int main()
{
	return cgl::launch(cgl_main);
}