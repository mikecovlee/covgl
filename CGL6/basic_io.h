#pragma once

// Covariant Graphics Library 6 Basic I/O

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
// Version: 6.16.7.11

#if __cplusplus < 201103L
#error Covariant C++ Library需要您的编译器支持C++11(C++0x)或者更高标准。请检查您否忘记了[-std=c++11]编译选项。
#else

// CGL6 I/O主文件
// Special Thanks
// Testing: Dxkite

#include "covstdlib.h"
#include "cgk3.h"
#include <cstdio>
#include <cstdlib>
#include <csignal>

namespace cov {
	namespace gl {
// 鼠标类
		class mouse {
		public:
			//鼠标事件
			//左键点击，右键点击，滚轮点击，滚轮向上，滚轮向下
			enum class events {
			    null,cursor_move,left_click,right_click,roller_click,roller_up,roller_down
			};
		protected:
			events mEvent=events::null;
			baseCtrl ** mCMap;
			std::size_t mWidth, mHeight, mCX, mCY;
		public:
			// 鼠标硬件类将不允许被复制
			mouse():mCMap(nullptr), mWidth(0), mHeight(0), mCX(0), mCY(0)
			{
			}
			mouse(std::size_t w, std::size_t h):mCMap(new baseCtrl *[w * h]), mWidth(w), mHeight(h), mCX(0), mCY(0)
			{
			}
			mouse(const mouse &) = delete;
			mouse(mouse &&) = delete;
			~mouse()
			{
				delete[]mCMap;
			}
			mouse & operator=(const mouse &) = delete;
			mouse & operator=(mouse &&) = delete;
			//事件获取
			events mouse_event() const
			{
				return mEvent;
			}
			//重新设置Map大小
			void resize(std::size_t w, std::size_t h)
			{
				delete[]mCMap;
				mCMap = new baseCtrl *[w * h];
				for(std::size_t i=0; i<w*h; ++i)
					mCMap[i]=nullptr;
				mWidth = w;
				mHeight = h;
			}
			//清空
			void clear()
			{
				if (mCMap != nullptr) {
					delete[]mCMap;
					mCMap = new baseCtrl *[mWidth * mHeight];
					for(std::size_t i=0; i<mWidth*mHeight; ++i)
						mCMap[i]=nullptr;
				}
			}
			//注册
			void login(const std::array < std::size_t, 2 > &posit, const std::array < std::size_t, 2 > &size, baseCtrl * ptr)
			{
				std::size_t col(posit[0]), row(posit[1]), sw(size[0]), sh(size[1]);
				if (mCMap == nullptr)
					throw std::logic_error(__func__);
				if (col < 0 || row < 0 || col > mWidth-1 || row > mHeight-1)
					throw std::out_of_range(__func__);
				for (std::size_t r = row; r < mHeight && r - row < sh; ++r)
					for (std::size_t c = col; c < mWidth && c - col < sw; ++c)
						mCMap[r * mWidth + c] = ptr;
			}
			//注销
			void logout(baseCtrl * ptr)
			{
				for (std::size_t i = 0; i < mWidth * mHeight; ++i)
					if (mCMap[i] == ptr)
						mCMap = nullptr;
			}
			//光标位置获取
			//请各位开发者劳神把鼠标的相对位置转换为绝对位置
			std::size_t cursor_x() const
			{
				return mCX;
			}
			std::size_t cursor_y() const
			{
				return mCY;
			}
			//激活鼠标事件
			void active(const std::array < std::size_t, 2 > &posit,events ev)
			{
				if(ev==events::null)
					throw std::invalid_argument(__func__);
				if (mCMap == nullptr)
					throw std::logic_error(__func__);
				if (posit[0] < 0 || posit[1] < 0 || posit[0] > mWidth-1 || posit[1] > mHeight -1)
					throw std::out_of_range(__func__);
				mCX=posit[0];
				mCY=posit[1];
				mEvent=ev;
				baseCtrl *obj = mCMap[mCY * mWidth + mCX];
				if(obj!=nullptr)
					obj->mouse_event.active(obj);
			}
		};
//输出方法基类
		class baseOutputMethod {
		public:
			baseOutputMethod()=default;
			baseOutputMethod(const baseOutputMethod&)=delete;
			baseOutputMethod(baseOutputMethod&&)=delete;
			virtual ~baseOutputMethod()=default;
			virtual void init()=0;
			virtual void stop()=0;
			virtual bool ready() const=0;
			virtual void update_image(const image&)=0;
		};
//基础输入输出类
		class basic_io {
		protected:
			mouse* mMouse=nullptr;
			keyboard* mKeyboard=nullptr;
			gamepad* mGamepad=nullptr;
			joystick* mJoystick=nullptr;
			baseOutputMethod* mOutput=nullptr;
			int mAverageTimeCoast=0;
			int mFrameLimit=0;
			int mFPS=0;
		public:
			basic_io()=default;
			basic_io(const basic_io&)=delete;
			basic_io(basic_io&&)=delete;
			~basic_io()=default;
			//目标尺寸获取
			static std::size_t get_target_width();
			static std::size_t get_target_height();
			// 信号处理
			static void force_exit(int);
			static void handle_segfault(int);
			//硬件注册
			void set_mouse(mouse* device)
			{
				mMouse=device;
			}
			void set_keyboard(keyboard* device)
			{
				mKeyboard=device;
			}
			void set_gamepad(gamepad* device)
			{
				mGamepad=device;
			}
			void set_joystick(joystick* device)
			{
				mJoystick=device;
			}
			void set_output_method(baseOutputMethod* device)
			{
				mOutput=device;
			}
			//输出方法的加载和停止
			void init_output_method()
			{
				if(mOutput==nullptr)
					throw std::runtime_error(__func__);
				signal(SIGSEGV,handle_segfault);
				signal(SIGINT,force_exit);
				signal(SIGABRT,force_exit);
				mOutput->init();
			}
			void stop_output_method()
			{
				if(mOutput==nullptr)
					throw std::runtime_error(__func__);
				signal(SIGSEGV,nullptr);
				signal(SIGINT,nullptr);
				signal(SIGABRT,nullptr);
				mOutput->stop();
			}
			bool output_method_ready() const
			{
				if(mOutput==nullptr)
					throw std::runtime_error(__func__);
				return mOutput->ready();
			}
			// 获取硬件指针
			mouse* mouse_device()
			{
				return mMouse;
			}
			keyboard* keyboard_device()
			{
				return mKeyboard;
			}
			gamepad* gamepad_device()
			{
				return mGamepad;
			}
			joystick* joystick_device()
			{
				return mJoystick;
			}
			mouse* const mouse_device() const
			{
				return mMouse;
			}
			keyboard* const keyboard_device() const
			{
				return mKeyboard;
			}
			gamepad* const gamepad_device() const
			{
				return mGamepad;
			}
			joystick* const joystick_device() const
			{
				return mJoystick;
			}
			// 获取帧率
			int fps() const
			{
				return mFPS;
			}
			// 帧率限制
			void frame_limit(unsigned short frame)
			{
				mFrameLimit=frame;
			}
			// 更新图像
			// 通过计算平均帧绘制花费时间并在此基础上对每一帧的绘制增加一点Delay时间，来避免低配置设备卡死
			void update_image(const image& img)
			{
				if(mOutput!=nullptr) {
					int time_coast(cov::clocks.measure(cov::timer::timeUnit::milliSec,[&] {mOutput->update_image(img);}));
					if(mFrameLimit==0) {
						if(mAverageTimeCoast!=0) {
							mAverageTimeCoast+=time_coast;
							mAverageTimeCoast/=2;
						} else
							mAverageTimeCoast=time_coast;
						if(mAverageTimeCoast*1.3-time_coast>0)
							cov::clocks.delay(cov::timer::timeUnit::milliSec,mAverageTimeCoast*1.3-time_coast);
						mFPS=1000/mAverageTimeCoast;
					} else {
						int delay_time=1000/mFrameLimit;
						if(delay_time-time_coast>0) {
							cov::clocks.delay(cov::timer::timeUnit::milliSec,delay_time-time_coast);
							mFPS=mFrameLimit;
						} else {
							if(mAverageTimeCoast!=0) {
								mAverageTimeCoast+=time_coast;
								mAverageTimeCoast/=2;
							} else
								mAverageTimeCoast=time_coast;
							if(mAverageTimeCoast*1.3-time_coast>0)
								cov::clocks.delay(cov::timer::timeUnit::milliSec,mAverageTimeCoast*1.3-time_coast);
							mFPS=1000/mAverageTimeCoast;
						}
					}
				}
			}
		};
		static basic_io ioctrl;
// 信号处理函数
		void basic_io::force_exit(int flag)
		{
			if(ioctrl.output_method_ready())
				ioctrl.stop_output_method();
			printf("CovGl have been exited safety.\n");
			exit(0);
		}
		void basic_io::handle_segfault(int flag)
		{
			if(ioctrl.output_method_ready())
				ioctrl.stop_output_method();
			printf("Your program have some problem about the Segmentation Fault.Please check your program after we terminate this program.\n");
			printf("CovGl have been exited safety.\n");
			exit(0);
		}
	}
}
#endif