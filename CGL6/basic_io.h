#pragma once
#include "cgk3.h"
#include "covstdlib.h"
#include <cstdio>
#include <csignal>
#include <cstdlib>
namespace cov {
	namespace gl {
// 鼠标类
		class mouse {
		public:
			//鼠标事件
			enum class events {
			    null,left_click,right_click,roller_click,roller_up,roller_down
			};
		protected:
			events mEvent=events::null;
			baseCtrl ** mCMap;
			std::size_t mWidth, mHeight, mCX, mCY;
		public:
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
				mWidth = w;
				mHeight = h;
			}
			//清空
			void clear()
			{
				if (mCMap != nullptr) {
					delete[]mCMap;
					mCMap = new baseCtrl *[mWidth * mHeight];
				}
			}
			//注册
			void login(const std::array < std::size_t, 2 > &posit, const std::array < std::size_t, 2 > &size, baseCtrl * ptr)
			{
				std::size_t col(posit[0]), row(posit[1]), sw(size[0]), sh(size[1]);
				if (mCMap == nullptr)
					throw std::logic_error(__func__);
				if (col < 0 || row < 0 || col > mWidth || row > mHeight)
					throw std::out_of_range(__func__);
				for (std::size_t r = row; r < mHeight && r - row < sh; ++r)
					for (std::size_t c = col; c < mWidth && c - col < sw; ++c)
						mCMap[r * mWidth + c] = ptr;
			}
			void login_with_image(const std::array < std::size_t, 2 > &posit, const image & img, baseCtrl * ptr)
			{
				login(posit, {img.width(), img.height()}, ptr);
			}
			//注销
			void logout(baseCtrl * ptr)
			{
				for (std::size_t i = 0; i < mWidth * mHeight; ++i)
					if (mCMap[i] == ptr)
						mCMap = nullptr;
			}
			//光标位置获取
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
				mOutput->init();
			}
			void stop_output_method()
			{
				if(mOutput==nullptr)
					throw std::runtime_error(__func__);
				signal(SIGSEGV,nullptr);
				signal(SIGINT,nullptr);
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
						if(mAverageTimeCoast*1.2-time_coast>0)
							cov::clocks.delay(cov::timer::timeUnit::milliSec,mAverageTimeCoast*1.2-time_coast);
						mFPS=1000/mAverageTimeCoast;
					} else {
						int delay_time=1000/mFrameLimit;
						if(delay_time-time_coast>0)
							cov::clocks.delay(cov::timer::timeUnit::milliSec,delay_time-time_coast);
						mFPS=mFrameLimit;
					}
				}
			}
		};
		static basic_io ioctrl;
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