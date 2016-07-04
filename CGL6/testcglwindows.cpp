#include "cgl6.h"
#include "cgl_linux.cpp"
bool KbHit()
{
	bool ret;
	fd_set fds;
	struct timeval tv;
	tv.tv_sec = 0;
	tv.tv_usec = 0;
	FD_ZERO(&fds);
	FD_SET(0, &fds);
	select(1, &fds, 0, 0, &tv);
	ret = FD_ISSET(0, &fds);
	return ret;
}
int GetKbHit()
{
	int ret;
	ret = getchar();
	return ret;
}
int main()
{
	using namespace cov::gl;
	scr.resize(basic_io::get_target_width(),basic_io::get_target_height());
	ioctrl.init_output_method();
	ioctrl.frame_limit(60);
	mouse cursor;
	scr.register_mouse_controller(&cursor);
	scr.cursor(pixel({true,false}, {colors::black,colors::white},'^'));
	bool mouse_clicked=false;
	int hcx(0),hcy(0);
	auto func=[&](baseClass* ptr) {
		if(ptr==nullptr)
			return;
		if(ptr->type()!=typeid(window))
			return;
		window* obj=dynamic_cast<window*>(ptr);
		switch(scr.mouse_controller()->mouse_event()) {
		case mouse::events::cursor_move:
			if(mouse_clicked) {
				if(scr.mouse_controller()->cursor_x()>=hcx)
					scr.focal_point()->posit()[0]+=scr.mouse_controller()->cursor_x()-hcx;
				else
					scr.focal_point()->posit()[0]-=hcx-scr.mouse_controller()->cursor_x();
				if(scr.mouse_controller()->cursor_y()>=hcy)
					scr.focal_point()->posit()[1]+=scr.mouse_controller()->cursor_y()-hcy;
				else
					scr.focal_point()->posit()[1]-=hcy-scr.mouse_controller()->cursor_y();
			}
			hcx=scr.mouse_controller()->cursor_x();
			hcy=scr.mouse_controller()->cursor_y();
			break;
		case mouse::events::left_click:
			scr.focus(obj);
			if(mouse_clicked)
				mouse_clicked=false;
			else
				mouse_clicked=true;
			break;
		}
	};
	window win0,win1,win2;
	win0.resize(20,15);
	win1.resize(20,15);
	win2.resize(20,15);
	win0.mouse_event.rebind(func);
	win1.mouse_event.rebind(func);
	win2.mouse_event.rebind(func);
	scr.login(&win0);
	scr.login(&win1);
	scr.login(&win2);
	scr.focus(&win0);
	win0.title("Window 1");
	win1.title("Window 2");
	win2.title("Window 3");
	win0.edge(pixel({true,false}, {colors::white,colors::blue},'*'));
	win1.edge(pixel({true,false}, {colors::white,colors::red},'*'));
	win2.edge(pixel({true,false}, {colors::white,colors::green},'*'));
	int cx(0),cy(0);
	while(true) {
		if(KbHit()) {
			switch(GetKbHit()) {
			case 'w':
				--cy;
				break;
			case 's':
				++cy;
				break;
			case 'a':
				--cx;
				break;
			case 'd':
				++cx;
				break;
			case 'x':
				scr.mouse_controller()->active({cx,cy},mouse::events::left_click);
				break;
			}
			scr.mouse_controller()->active({cx,cy},mouse::events::cursor_move);
		}
		scr.resize(basic_io::get_target_width(),basic_io::get_target_height());
		scr.fill(pixel({true,false}, {colors::black,colors::white},' '));
		scr.render();
		ioctrl.update_image(scr.surface());
	}
	return 0;
}