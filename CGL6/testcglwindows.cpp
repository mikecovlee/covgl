#include "cgl6.h"
#include "cgl_linux.cpp"
#include <sstream>
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
template<typename T>
std::string toString(T val)
{
	std::stringstream ss;
	ss<<val;
	std::string ret;
	ss>>ret;
	return std::move(ret);
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
	std::list<window*> windows;
	auto func=[&](baseClass* ptr) {
		if(ptr==nullptr)
			return;
		if(ptr->type()!=typeid(window))
			return;
		window* obj=dynamic_cast<window*>(ptr);
		switch(scr.mouse_controller()->mouse_event()) {
		case mouse::events::cursor_move:
			if(mouse_clicked&&scr.focal_point()!=nullptr) {
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
		case mouse::events::right_click:
			obj->hide();
			break;
		}
	};
	button create;
	create.text("New Window");
	int windows_count=1;
	int wx(1),wy(5);
	create.mouse_event.rebind([&](baseClass* ptr) {
		if(ptr==&create&&scr.mouse_controller()->mouse_event()==mouse::events::left_click) {
			window* win=new window;
			win->resize(20,10);
			win->posit({wx,wy});
			win->title("Window "+toString(windows_count));
			switch(windows_count%6) {
			case 0:
				win->edge(pixel({true,false}, {colors::white,colors::red},'*'));
				break;
			case 1:
				win->edge(pixel({true,false}, {colors::white,colors::green},'*'));
				break;
			case 2:
				win->edge(pixel({true,false}, {colors::white,colors::blue},'*'));
				break;
			case 3:
				win->edge(pixel({true,false}, {colors::white,colors::magenta},'*'));
				break;
			case 4:
				win->edge(pixel({true,false}, {colors::white,colors::yellow},'*'));
				break;
			case 5:
				win->edge(pixel({true,false}, {colors::white,colors::cyan},'*'));
				break;
			}
			win->mouse_event.rebind(func);
			scr.login(win);
			scr.focus(win);
			windows.push_back(win);
			++windows_count;
			wx+=3;
			wy+=3;
		}
	});
	create.posit({1,1});
	scr.login(&create);
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
			case 'c':
				scr.mouse_controller()->active({cx,cy},mouse::events::right_click);
				break;
			}
			scr.mouse_controller()->active({cx,cy},mouse::events::cursor_move);
		}
		scr.resize(basic_io::get_target_width(),basic_io::get_target_height());
		scr.fill(pixel({true,false}, {colors::black,colors::white},' '));
		scr.render();
		ioctrl.update_image(scr.surface());
		for(auto&it:windows) {
			if(!it->visable()) {
				scr.logout(it);
				delete it;
				windows.remove(it);
				break;
			}
		}
	}
	for(auto&it:windows)
		delete it;
	return 0;
}