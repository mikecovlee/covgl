#include "CGL/CGL5.h"
int main()
{
	return cgl::launch([&]
	{
		cgl::scr.back(cgl::color::white);
		cgl::text hello("Hello ,CGL");
		hello.text = cgl::color::blue;
		hello.back = cgl::scr.back();
		cgl::scr << hello;
		while (true)
		{
			cgl::out << cgl::scr;
		}
		return 0;
	});
}