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
		cov::clocks.delay(cov::timer::timeUnit::milliSec,1000/ioctrl.fps());
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
	bool running=true;
	scr.resize(basic_io::get_target_width(),basic_io::get_target_height());
	ioctrl.init_output_method();
	ioctrl.frame_limit(30);
	// 初始化鼠标和窗口管理器
	mouse cursor;
	scr.register_mouse_controller(&cursor);
	scr.cursor(pixel({true,false}, {colors::black,colors::white},'^'));
	std::list<window*> windows;
	bool move=false;
	//菜单
	menu scr_main_menu;
	menu win_main_menu;
	menu colors_menu;
	scr_main_menu.hide();
	win_main_menu.hide();
	colors_menu.hide();
	colors_menu.text("Colors");
	//菜单标签
	lable smm_newwin("New");
	lable smm_about("About");
	lable smm_exit("Exit");
	lable wmm_move("Move");
	lable wmm_info("Info");
	lable wmm_close("Close");
	lable cm_red("Red",pixel({true,true}, {colors::red,colors::white},' '));
	lable cm_green("Green",pixel({true,true}, {colors::green,colors::white},' '));
	lable cm_blue("Blue",pixel({true,true}, {colors::blue,colors::white},' '));
	lable cm_magenta("Magenta",pixel({true,true}, {colors::magenta,colors::white},' '));
	lable cm_cyan("Cyan",pixel({true,true}, {colors::cyan,colors::white},' '));
	lable cm_yellow("Yellow",pixel({true,true}, {colors::yellow,colors::white},' '));
	//鼠标事件处理
	smm_exit.mouse_event.rebind([&](baseClass* obj) {
		if(scr.mouse_controller()->mouse_event()==mouse::events::left_click) {
			running=false;
		}
	});
	scr_main_menu.mouse_event.rebind([&](baseClass* ptr) {
		menu* obj=dynamic_cast<menu*>(ptr);
		obj->mouse_controller()->active({scr.mouse_controller()->cursor_x()-obj->posit()[0],scr.mouse_controller()->cursor_y()-obj->posit()[1]},scr.mouse_controller()->mouse_event());
	});
	wmm_move.mouse_event.rebind([&](baseClass* obj) {
		if(win_main_menu.mouse_controller()->mouse_event()==mouse::events::left_click) {
			if(move) {
				move=false;
				wmm_move.text("Move");
			} else {
				move=true;
				wmm_move.text("Dock");
			}
		}
	});
	wmm_close.mouse_event.rebind([&](baseClass* obj) {
		if(win_main_menu.mouse_controller()->mouse_event()==mouse::events::left_click) {
			scr.focal_point()->hide();
			win_main_menu.hide();
		}
	});
	win_main_menu.mouse_event.rebind([&](baseClass* ptr) {
		colors_menu.hide();
		menu* obj=dynamic_cast<menu*>(ptr);
		obj->mouse_controller()->active({scr.mouse_controller()->cursor_x()-obj->posit()[0],scr.mouse_controller()->cursor_y()-obj->posit()[1]},scr.mouse_controller()->mouse_event());
	});
	cm_red.mouse_event.rebind([&](baseClass* ptr) {
		if(scr.mouse_controller()->mouse_event()==mouse::events::left_click) {
			dynamic_cast<window*>(scr.focal_point())->edge(pixel({true,false}, {colors::white,colors::red},'*'));
			colors_menu.hide();
			win_main_menu.hide();
		}
	});
	cm_green.mouse_event.rebind([&](baseClass* ptr) {
		if(scr.mouse_controller()->mouse_event()==mouse::events::left_click) {
			dynamic_cast<window*>(scr.focal_point())->edge(pixel({true,false}, {colors::white,colors::green},'*'));
			colors_menu.hide();
			win_main_menu.hide();
		}
	});
	cm_blue.mouse_event.rebind([&](baseClass* ptr) {
		if(scr.mouse_controller()->mouse_event()==mouse::events::left_click) {
			dynamic_cast<window*>(scr.focal_point())->edge(pixel({true,false}, {colors::white,colors::blue},'*'));
			colors_menu.hide();
			win_main_menu.hide();
		}
	});
	cm_magenta.mouse_event.rebind([&](baseClass* ptr) {
		if(scr.mouse_controller()->mouse_event()==mouse::events::left_click) {
			dynamic_cast<window*>(scr.focal_point())->edge(pixel({true,false}, {colors::white,colors::magenta},'*'));
			colors_menu.hide();
			win_main_menu.hide();
		}
	});
	cm_cyan.mouse_event.rebind([&](baseClass* ptr) {
		if(scr.mouse_controller()->mouse_event()==mouse::events::left_click) {
			dynamic_cast<window*>(scr.focal_point())->edge(pixel({true,false}, {colors::white,colors::cyan},'*'));
			colors_menu.hide();
			win_main_menu.hide();
		}
	});
	cm_yellow.mouse_event.rebind([&](baseClass* ptr) {
		if(scr.mouse_controller()->mouse_event()==mouse::events::left_click) {
			dynamic_cast<window*>(scr.focal_point())->edge(pixel({true,false}, {colors::white,colors::yellow},'*'));
			colors_menu.hide();
			win_main_menu.hide();
		}
	});
	colors_menu.mouse_event.rebind([&](baseClass* ptr) {
		menu* obj=dynamic_cast<menu*>(ptr);
		if(scr.mouse_controller()->cursor_x()<win_main_menu.posit()[0]+win_main_menu.real_width()&&
		        scr.mouse_controller()->cursor_y()<win_main_menu.posit()[1]+win_main_menu.real_height()) {
			obj->posit({win_main_menu.posit()[0]+win_main_menu.real_width(),scr.mouse_controller()->cursor_y()});
			obj->show();
			scr.front(obj);
		} else {
			obj->mouse_controller()->active({scr.mouse_controller()->cursor_x()-obj->posit()[0],scr.mouse_controller()->cursor_y()-obj->posit()[1]},scr.mouse_controller()->mouse_event());
		}
	});
	//将标签添加至菜单
	scr_main_menu.add_element(&smm_newwin);
	scr_main_menu.add_element(&smm_about);
	scr_main_menu.add_element(&smm_exit);
	win_main_menu.add_element(&wmm_move);
	win_main_menu.add_element(&wmm_info);
	win_main_menu.add_element(&colors_menu);
	win_main_menu.add_element(&wmm_close);
	colors_menu.add_element(&cm_red);
	colors_menu.add_element(&cm_green);
	colors_menu.add_element(&cm_blue);
	colors_menu.add_element(&cm_magenta);
	colors_menu.add_element(&cm_cyan);
	colors_menu.add_element(&cm_yellow);
	// 注册菜单
	scr.login(&scr_main_menu);
	scr.login(&win_main_menu);
	scr.login(&colors_menu);
	// 窗口触摸事件处理
	auto func=[&](baseClass* ptr) {
		if(ptr==nullptr)
			return;
		if(ptr->type()!=typeid(window))
			return;
		window* obj=dynamic_cast<window*>(ptr);
		switch(scr.mouse_controller()->mouse_event()) {
		case mouse::events::cursor_move:
			break;
		case mouse::events::left_click:
			move=false;
			wmm_move.text("Move");
			break;
		case mouse::events::right_click:
			win_main_menu.posit({scr.mouse_controller()->cursor_x()+1,scr.mouse_controller()->cursor_y()});
			scr.front(&win_main_menu);
			scr.focal_point(obj);
			win_main_menu.show();
			break;
		}
	};
	int windows_count=1;
	smm_newwin.mouse_event.rebind([&](baseClass* obj) {
		if(scr.mouse_controller()->mouse_event()==mouse::events::left_click) {
			window* win=new window;
			win->resize(20,10);
			win->posit({scr.mouse_controller()->cursor_x(),scr.mouse_controller()->cursor_y()});
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
		}
	});
	// 主循环
	int cx(0),cy(0);
	while(running) {
		//控制鼠标
		if(KbHit()) {
			switch(GetKbHit()) {
			case 'w':
				--cy;
				if(move) --scr.focal_point()->posit()[1];
				break;
			case 's':
				++cy;
				if(move) ++scr.focal_point()->posit()[1];
				break;
			case 'a':
				--cx;
				if(move) --scr.focal_point()->posit()[0];
				break;
			case 'd':
				++cx;
				if(move) ++scr.focal_point()->posit()[0];
				break;
			case 'x':
				colors_menu.hide();
				win_main_menu.hide();
				scr_main_menu.hide();
				scr.mouse_controller()->active({cx,cy},mouse::events::left_click);
				break;
			case 'c':
				colors_menu.hide();
				win_main_menu.hide();
				scr_main_menu.hide();
				if(!scr.mouse_controller()->active({cx,cy},mouse::events::right_click)) {
					scr_main_menu.posit({scr.mouse_controller()->cursor_x()+1,scr.mouse_controller()->cursor_y()});
					scr_main_menu.show();
					scr.front(&scr_main_menu);
				}
				break;
			}
			scr.mouse_controller()->active({cx,cy},mouse::events::cursor_move);
		}
		// 绘制
		scr.resize(basic_io::get_target_width(),basic_io::get_target_height());
		scr.fill(pixel({true,false}, {colors::white,colors::black},' '));
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
	ioctrl.stop_output_method();
	return 0;
}