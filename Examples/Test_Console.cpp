#include "CGL/CGL5.h"


static cov::atomic < char >input;


CreateThread(Input)
{
	SitLoop(Input)
	{
		OnRunningSit
		{
			input.store(cgl::out.GetKbHit());
			Thread(Input).pause();
			EndSit;
		}
		EndSitLoop(Input);
	}
	EndThread(Input, ThreadSuccess);
}


CreateThread(Main)
{
	cgl::window win(cgl::scr);
	win.title("CGL Terminal");
	win.back(cgl::color::white);
	win.render_back(cgl::color::black);
	win.border_color(cgl::color::black);
	win.border(' ');
	cgl::console < cgl::window > con(win);
	win.show();
	win.focus();
	con << "Covariant GL 5 Terminal Emulator" << cgl::ioctrl::endl << "$:/>";
	SitLoop(Main)
	{
		OnRunningSit
		{
			if (Thread(Input).state() == cov::thread::states::onPause)
			{
				con.input_mod(true);
				con << (char)input;
				if (!con.input_mod())
				{
					std::string input = con.input();
					if (input == "clear")
						con.ClrScr();
					if (input == "exit")
						BreakOffLoop(Main);
					con << "$:/>";
				}
				Thread(Input).resume();
			}
			else
				con << cgl::ioctrl::flush;
			EndSit;
		}
		EndSitLoop(Main);
	}
	EndThread(Main, ThreadSuccess);
}


void start()
{
	Thread(Input).detach();
	Thread(Main).detach();
	printf("Wait for threads call back...\n");
	fflush(stdout);
	WaitForLeisure(Main);
	printf("Call back Main.\n");
	fflush(stdout);
	WaitForLeisure(Input);
	printf("Call back Input.\n");
	fflush(stdout);
	Thread(Input).resume();
	Thread(Main).resume();
	WaitForExit(Main);
	Thread(Input).suspend();
}


int main()
{
	return cgl::launch(start);
}