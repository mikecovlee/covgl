#include "cgl_linux.cpp"
#include "cgl_character.h"
#include <thread>
#include <mutex>
#include <list>
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
	ioctrl.init_output_method();
	ioctrl.frame_limit(30);
	image img(basic_io::get_target_width(),basic_io::get_target_height());
	auto draw_ch=[&](const int* ch,int w,int h,int x,int y) {
		for(int r=0; r<h; ++r) {
			for(int c=0; c<w; ++c) {
				if(ch[r*6+c]==1)
					img.draw({c+x,r+y},pixel({true,false}, {colors::black,colors::black},' '));
			}
		}
	};
	std::list<char> input;
	std::mutex locker;
	std::thread inth([&] {
		while(true)
		{
			char in=GetKbHit();
			locker.lock();
			input.push_back(in);
			locker.unlock();
		}
	});
	inth.detach();
	std::list<const int*>chs;
	auto draw_chs=[&] {
		int w=2*(chs.size()-1)+6*chs.size();
		int h=1;
		int x(1),y(0);
		h+=(w-w%img.width())/img.width();
		y=0.5*img.height()-2.5*h-(h-1);
		for(auto&it:chs)
		{
			draw_ch(it,6,5,x,y);
			if(x+8>=img.width()-1) {
				y+=6;
				x=1;
			} else
				x+=8;
		}
	};
	int frame_count(0);
	while(true) {
		img.resize(basic_io::get_target_width(),basic_io::get_target_height());
		img.fill(pixel({true,false}, {colors::white,colors::white},' '));
		if(frame_count<=4) {
			chs.push_back(single_ch::underline);
			draw_chs();
			chs.pop_back();
			++frame_count;
		} else {
			if(frame_count>8)
				frame_count=0;
			else
				++frame_count;
			chs.push_back(single_ch::null);
			draw_chs();
			chs.pop_back();
		}
		ioctrl.update_image(img);
		if(!input.empty()) {
			char in=input.front();
			input.pop_front();
			switch(std::tolower(in)) {
			case ',':
				chs.push_back(single_ch::comma);
				break;
			case '.':
				chs.push_back(single_ch::dot);
				break;
			case ';':
				chs.push_back(single_ch::semi);
				break;
			case ':':
				chs.push_back(single_ch::colon);
				break;
			case '!':
				chs.push_back(single_ch::exc);
				break;
			case '?':
				chs.push_back(single_ch::ques);
				break;
			case ' ':
				chs.push_back(single_ch::null);
				break;
			case '_':
				chs.push_back(single_ch::underline);
				break;
			case '(':
				chs.push_back(single_ch::left_bracket);
				break;
			case ')':
				chs.push_back(single_ch::right_bracket);
				break;
			case '0':
				chs.push_back(single_ch::num0);
				break;
			case '1':
				chs.push_back(single_ch::num1);
				break;
			case '2':
				chs.push_back(single_ch::num2);
				break;
			case '3':
				chs.push_back(single_ch::num3);
				break;
			case '4':
				chs.push_back(single_ch::num4);
				break;
			case '5':
				chs.push_back(single_ch::num5);
				break;
			case '6':
				chs.push_back(single_ch::num6);
				break;
			case '7':
				chs.push_back(single_ch::num7);
				break;
			case '8':
				chs.push_back(single_ch::num8);
				break;
			case '9':
				chs.push_back(single_ch::num9);
				break;
			case 'a':
				chs.push_back(single_ch::A);
				break;
			case 'b':
				chs.push_back(single_ch::B);
				break;
			case 'c':
				chs.push_back(single_ch::C);
				break;
			case 'd':
				chs.push_back(single_ch::D);
				break;
			case 'e':
				chs.push_back(single_ch::E);
				break;
			case 'f':
				chs.push_back(single_ch::F);
				break;
			case 'g':
				chs.push_back(single_ch::G);
				break;
			case 'h':
				chs.push_back(single_ch::H);
				break;
			case 'i':
				chs.push_back(single_ch::I);
				break;
			case 'j':
				chs.push_back(single_ch::J);
				break;
			case 'k':
				chs.push_back(single_ch::K);
				break;
			case 'l':
				chs.push_back(single_ch::L);
				break;
			case 'm':
				chs.push_back(single_ch::M);
				break;
			case 'n':
				chs.push_back(single_ch::N);
				break;
			case 'o':
				chs.push_back(single_ch::O);
				break;
			case 'p':
				chs.push_back(single_ch::P);
				break;
			case 'q':
				chs.push_back(single_ch::Q);
				break;
			case 'r':
				chs.push_back(single_ch::R);
				break;
			case 's':
				chs.push_back(single_ch::S);
				break;
			case 't':
				chs.push_back(single_ch::T);
				break;
			case 'u':
				chs.push_back(single_ch::U);
				break;
			case 'v':
				chs.push_back(single_ch::V);
				break;
			case 'w':
				chs.push_back(single_ch::W);
				break;
			case 'x':
				chs.push_back(single_ch::X);
				break;
			case 'y':
				chs.push_back(single_ch::Y);
				break;
			case 'z':
				chs.push_back(single_ch::Z);
				break;
			case 127:
				if(!chs.empty())
					chs.pop_back();
				break;
			}
		}
	}
}