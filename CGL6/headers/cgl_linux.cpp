#define CGL_CONSOLE

// Covariant Graphics Library 6 Linux I/O Adaptation

// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// any later version.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

// Copyright (C) 2016 Mike Covariant Lee(李登淳)
// Version: 6.16.7.10

#include "basic_io.h"
#include <iostream>
#include <string>
#include <unistd.h>
#include <termios.h>
#include <sys/ioctl.h>
#include <sys/select.h>

namespace cov {
	namespace gl {
// 终端尺寸获取
		std::size_t basic_io::get_target_width()
		{
			struct winsize size;
			ioctl(STDIN_FILENO, TIOCGWINSZ, &size);
			return size.ws_col;
		}
		std::size_t basic_io::get_target_height()
		{
			struct winsize size;
			ioctl(STDIN_FILENO, TIOCGWINSZ, &size);
			return size.ws_row-1;
		}
// Linux输出方法类
		class linuxOutputMethod:public baseOutputMethod {
		protected:
			bool mReady = false;
			void echo(bool);
			void clrscr()
			{
				std::cout<<"\e[2J\e[0;0f"<<std::flush;
			}
			void rst_attri()
			{
				std::cout<<"\e[37;40;0m"<<std::flush;
			}
			void rst_cursor()
			{
				std::cout<<"\e[0;0f"<<std::flush;
			}
		public:
			linuxOutputMethod()
			{
				ioctrl.set_output_method(this);
			}
			void init() override
			{
				rst_attri();
				clrscr();
				mReady = true;
				std::ios::sync_with_stdio(false);
				echo(false);
			}
			void stop() override
			{
				rst_attri();
				mReady = false;
				std::ios::sync_with_stdio(true);
				echo(true);
			}
			bool ready() const override
			{
				return mReady;
			}
			void update_image(const image&) override;
		};
		static linuxOutputMethod linux_output_method;
		void linuxOutputMethod::echo(bool in)
		{
			std::cout << std::flush;
			struct termios oldt, newt;
			tcgetattr(0, &oldt);
			newt = oldt;
			if (in) {
				std::cout << "\33[?25h" << std::flush;
				newt.c_lflag |= ECHO;
				newt.c_lflag |= ICANON;
			} else {
				std::cout << "\33[?25l" << std::flush;
				newt.c_lflag &= ~ECHO;
				newt.c_lflag &= ~ICANON;
			}
			tcsetattr(0, TCSANOW, &newt);
		}
		void linuxOutputMethod::update_image(const image& img)
		{
			rst_cursor();
			const std::size_t sw(basic_io::get_target_width()),sh(basic_io::get_target_height());
			for(std::size_t y=0; y<img.height()&&y<sh; ++y) {
				for(std::size_t x=0; x<img.width()&&x<sw; ++x) {
					const pixel& pix=img.at({x,y});
					std::string cmd="\e[0m\e[";
					switch(pix.color()[0]) {
					case colors::white:
						cmd+="37;";
						break;
					case colors::black:
						cmd+="30;";
						break;
					case colors::red:
						cmd+="31;";
						break;
					case colors::green:
						cmd+="32;";
						break;
					case colors::blue:
						cmd+="34;";
						break;
					case colors::magenta:
						cmd+="35;";
						break;
					case colors::yellow:
						cmd+="33;";
						break;
					case colors::cyan:
						cmd+="36;";
						break;
					default:
						throw std::logic_error(__func__);
						break;
					}
					switch(pix.color()[1]) {
					case colors::white:
						cmd+="47";
						break;
					case colors::black:
						cmd+="40";
						break;
					case colors::red:
						cmd+="41";
						break;
					case colors::green:
						cmd+="42";
						break;
					case colors::blue:
						cmd+="44";
						break;
					case colors::magenta:
						cmd+="45";
						break;
					case colors::yellow:
						cmd+="43";
						break;
					case colors::cyan:
						cmd+="46";
						break;
					default:
						throw std::logic_error(__func__);
						break;
					}
					if(pix.attri()[0])
						cmd+=";1";
					if(pix.attri()[1])
						cmd+=";4";
					cmd+="m";
					std::cout<<cmd<<pix.image();
				}
				rst_attri();
				std::cout<<std::endl;
			}
		}
	}
}