#include "cgl6.h"
#include "cgl_linux.cpp"
#include <sstream>
//由于CGL6键盘驱动尚未完成，暂时使用Linux Termios
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
// 字符串转换函数
template<typename T>
std::string toString(T val)
{
	std::stringstream ss;
	ss<<val;
	std::string ret;
	ss>>ret;
	return std::move(ret);
}
/*
* 窗口主线程
* 请注意窗口线程的设计规范:
* 1:必须只接收一个cov::gl::window指针作为参数
* 2:主循环必须以cov::gl::window::thread_running为循环条件
* 3:线程结尾处必须调用一次cov::gl::window::thread_stopped发送结束信号
* 4:务必为共用资源加锁
* 务必遵守线程设计规范！如不遵守规范将可能会发生难以调试的未定义行为
*/
void winthread(cov::gl::window* win)
{
	using namespace cov::gl;
	int x(0),y(0);
	bool x_grow(true),y_grow(true);
	std::mutex locker;
	// 线程主循环
	while(win->thread_running()) {
		cov::clocks.delay(cov::timer::timeUnit::milliSec,35);
		if(x_grow)
			++x;
		else
			--x;
		if(y_grow)
			++y;
		else
			--y;
		if(x<=0)
			x_grow=true;
		if(x>=win->width()-1)
			x_grow=false;
		if(y<=0)
			y_grow=true;
		if(y>=win->height()-1)
			y_grow=false;
		locker.lock();
		win->clear();
		win->draw({x,y},pixel({true,false}, {colors::black,colors::white},' '));
		win->render();
		locker.unlock();
	}
	// 发送结束信号
	win->thread_stopped();
}
int main()
{
	using namespace cov::gl;
	// 初始化I/O
	scr.resize(basic_io::get_target_width(),basic_io::get_target_height());
	ioctrl.init_output_method();
	ioctrl.frame_limit(60);
	// 初始化鼠标和窗口管理器
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
	//窗口管理器按钮
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
			win->main_thread(winthread);
			++windows_count;
			wx+=3;
			wy+=3;
		}
	});
	create.posit({1,1});
	scr.login(&create);
	// 主循环
	int cx(0),cy(0);
	while(true) {
		//控制鼠标
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
		// 绘制
		scr.resize(basic_io::get_target_width(),basic_io::get_target_height());
		scr.fill(pixel({true,false}, {colors::black,colors::white},' '));
		scr.render();
		ioctrl.update_image(scr.surface());
		// 窗口管理器
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