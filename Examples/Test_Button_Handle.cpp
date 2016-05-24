#include "CGL/CGL5.h"


int start()
{
	using namespace cgl;
	scr.back(color::blue);
	text txt0("Press ok button to continue...");
	text txt1("Hello,Covariant GL V!");
	txt0.show();
	txt1.show();
	txt0.back = txt1.back = scr.back();
	button ok;
	ok.txt("ok");
	ok.show();
	ok.select();
	ok.resize_from_txt();
	ok.resize(scr.width() - 8, ok.high());
	bool clicked = false;
	auto print =[&]{
		scr.reset();
		ok.resize(scr.width() - 8, ok.high());
		txt0.x = scr.width() / 2 - txt0.size() / 2;
		txt1.x = scr.width() / 2 - txt1.size() / 2;
		ok.x = scr.width() / 2 - ok.width() / 2;
		txt0.y = txt1.y = 2;
		ok.y = scr.high() - ok.high() - 2;
		if (ok.onButtonClick.demand())
		{
			txt1.show();
			txt0.hide();
		}
		else
		{
			txt0.show();
			txt1.hide();
		}
		if (clicked)
			ok.click();
		ControlManager.display(scr, out);
	};
	unsigned int begin, end;
	while (true)
	{
		print();
		if (clicked)
		{
			end = cgl::clock.rt_sec();
			if ((end - begin) > 2)
				clicked = false;
		}
		if (out.GetKbHitPer() == keyboard::num_select)
		{
			begin = cgl::clock.rt_sec();
			clicked = true;
			ok.border('-');
			cgl::clock.delay(cgl::time::ms, 50000, print);
			ok.border(' ');
		}
	}
	return 0;
}


int main()
{
	return cgl::launch(start);
}