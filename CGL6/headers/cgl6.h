#pragma once

// Covariant Graphics Library 6

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
// Version: 6.16.7.11 Beta

#if __cplusplus < 201103L
#error Covariant C++ Library需要您的编译器支持C++11(C++0x)或者更高标准。请检查您否忘记了[-std=c++11]编译选项。
#else

// CGL6 Console 主文件

#define CGL_CONSOLE

#include "cgk3.h"
#include "basic_io.h"
#include <thread>
#include <atomic>
#include <mutex>

namespace cov {
	namespace gl {
// 屏幕类
		class screen:virtual public baseActivity,virtual public image {
		protected:
			// 硬件控制器指针
			mouse* mMouse=nullptr;
			keyboard* mKeyboard=nullptr;
			gamepad* mGamepad=nullptr;
			joystick* mJoystick=nullptr;
			basic_io* mIoctrl=nullptr;
			// 其他成员
			bool mHideCursor=false;
			pixel mCursor;
			image mRenderer;
		public:
			screen()=default;
			screen(basic_io* ptr):mIoctrl(ptr) {}
			virtual ~screen()=default;
			screen(const screen&)=delete;
			screen(screen&&)=delete;
			screen& operator=(const screen&)=delete;
			screen& operator=(screen&&)=delete;
			// 鼠标指针图像接口
			// 当一个可用的鼠标设备被注册后，鼠标指针就会显示
			// 你也可以手动设置不显示鼠标指针
			void cursor(const pixel& pix)
			{
				this->mCursor=pix;
			}
			pixel& cursor()
			{
				return this->mCursor;
			}
			const pixel& cursor() const
			{
				return this->mCursor;
			}
			void show_cursor()
			{
				this->mHideCursor=false;
			}
			void hide_cursor()
			{
				this->mHideCursor=true;
			}
			// 设备接口
			void register_io_controller(basic_io* device)
			{
				mIoctrl=device;
			}
			void register_mouse_controller(mouse* device)
			{
				mMouse=device;
			}
			void register_keyboard_controller(keyboard* device)
			{
				mKeyboard=device;
			}
			void register_gamepad_controller(gamepad* device)
			{
				mGamepad=device;
			}
			void register_joystick_controller(joystick* device)
			{
				mJoystick=device;
			}
			virtual mouse* mouse_controller() override
			{
				return this->mMouse;
			}
			virtual mouse* const mouse_controller() const override
			{
				return this->mMouse;
			}
			virtual keyboard* keyboard_controller() override
			{
				return this->mKeyboard;
			}
			virtual keyboard* const keyboard_controller() const override
			{
				return this->mKeyboard;
			}
			virtual gamepad* gamepad_controller() override
			{
				return this->mGamepad;
			}
			virtual gamepad* const gamepad_controller() const override
			{
				return this->mGamepad;
			}
			virtual joystick* joystick_controller() override
			{
				return this->mJoystick;
			}
			virtual joystick* const joystick_controller() const override
			{
				return this->mJoystick;
			}
			// 绘制接口
			virtual const image& surface() const override
			{
				return mRenderer;
			}
			virtual void render() override;
		};
// Window控件类
		class window:public baseCtrl,public baseActivity,public image {
		protected:
			//属性
			pixel mEdge;
			std::string mTitle;
			//绘制与线程
			image mRenderer;
			image mFrameCache;
			std::thread* mThread=nullptr;
			std::atomic<bool> mThreadRunning;
			std::atomic<bool> mThreadStopped;
			std::mutex mLocker;
		public:
			window():mThreadRunning(false),mThreadStopped(true) {}
			window(const window&)=delete;
			window(window&&)=delete;
			window& operator=(const window&)=delete;
			window& operator=(window&&)=delete;
			virtual ~window()
			{
				if(mThread!=nullptr) {
					mThreadRunning=false;
					while(!mThreadStopped);
					delete mThread;
				}
			}
			// 属性设置
			void edge(const pixel& pix)
			{
				this->mEdge=pix;
			}
			pixel& edge()
			{
				return this->mEdge;
			}
			const pixel& edge() const
			{
				return this->mEdge;
			}
			void title(const std::string& str)
			{
				this->mTitle=str;
			}
			std::string& title()
			{
				return this->mTitle;
			}
			const std::string& title() const
			{
				return this->mTitle;
			}
			// 线程接口
			bool thread_running() const
			{
				return this->mThreadRunning;
			}
			// 此函数只能在线程结束时调用，请勿随意调用！会产生不可预期的后果！
			bool thread_stopped()
			{
				this->mThreadStopped=true;
			}
			template<typename T,typename...Argt> void main_thread(T func,Argt&&...args)
			{
				if(mThread!=nullptr) {
					mThreadRunning=false;
					while(!mThreadStopped);
					delete mThread;
				}
				mThread=new std::thread(func,this,args...);
				mThreadRunning=true;
				mThreadStopped=false;
				mThread->detach();
			}
			// 图形接口
			virtual std::size_t real_width() const
			{
				if(this->usable())
					return this->width()+2;
				else
					throw std::logic_error(__func__);
			}
			virtual std::size_t real_height() const
			{
				if(this->usable())
					return this->height()+4;
				else
					throw std::logic_error(__func__);
			}
			virtual const image& surface() const override
			{
				return this->mFrameCache;
			}
			virtual void render() override;
		};
		class button:public baseCtrl {
		protected:
			image mRenderer;
			std::string mText;
			pixel mEdge;
		public:
			button():mText("button"),mEdge({true,true}, {colors::black,colors::white},'#') {}
			button(const button&)=default;
			button(button&&)=default;
			virtual ~button()=default;
			// 属性设置接口
			void text(const std::string& str)
			{
				this->mText=str;
			}
			std::string& text()
			{
				return this->mText;
			}
			const std::string& text() const
			{
				return this->mText;
			}
			void edge(const pixel& pix)
			{
				this->mEdge=pix;
			}
			pixel& edge()
			{
				return this->mEdge;
			}
			const pixel& edge() const
			{
				return this->mEdge;
			}
			// 图形接口
			virtual std::size_t real_width() const
			{
				return this->mText.size()+4;
			}
			virtual std::size_t real_height() const
			{
				return 3;
			}
			virtual const image& surface() const override
			{
				return this->mRenderer;
			}
			virtual void render() override;
		};
		class lable:public baseCtrl {
		protected:
			std::string mText;
			pixel mAttri;
			image mImg;
		public:
			lable():mText("lable"),mAttri({true,true}, {colors::black,colors::white},' ') {}
			lable(const std::string& str):mText(str),mAttri({true,true}, {colors::black,colors::white},' ') {}
			lable(const std::string& str,const pixel& pix):mText(str),mAttri(pix) {}
			virtual ~lable()=default;
			void text(const std::string& str)
			{
				this->mText=str;
			}
			std::string& text()
			{
				return this->mText;
			}
			const std::string& text() const
			{
				return this->mText;
			}
			void attri(const pixel& pix)
			{
				this->mAttri=pix;
			}
			pixel& attri()
			{
				return this->mAttri;
			}
			const pixel& attri() const
			{
				return this->mAttri;
			}
			virtual std::size_t real_width() const override
			{
				return this->mText.size();
			}
			virtual std::size_t real_height() const override
			{
				return 1;
			}
			virtual const image& surface() const override
			{
				return this->mImg;
			}
			virtual void render()
			{
				this->mImg.resize(this->real_width(),this->real_height());
				for(std::size_t x; x<this->mImg.width(); ++x) {
					this->mAttri.image(this->mText.at(x));
					this->mImg.draw({x,0},this->mAttri);
				}
			}
		};
		class menu:public lable {
		protected:
			mouse mMouse;
			std::list<lable*> mElements;
		public:
			menu()=default;
			virtual ~menu()=default;
			mouse* mouse_controller()
			{
				return &this->mMouse;
			}
			const mouse* mouse_controller() const
			{
				return &this->mMouse;
			}
			void add_element(lable* obj)
			{
				if(obj!=nullptr)
					this->mElements.push_back(obj);
			}
			void remove_element(lable* obj)
			{
				this->mElements.remove(obj);
			}
			virtual std::size_t real_width() const override
			{
				std::size_t max_size(0);
				for(auto&it:this->mElements)
					if(it!=nullptr&&it->text().size()>max_size)
						max_size=it->text().size();
				return max_size+2;
			}
			virtual std::size_t real_height() const override
			{
				return this->mElements.size();
			}
			virtual void render() override;
		};
// 对象定义和类外实现
		static screen scr(&ioctrl);
		void screen::render()
		{
			mRenderer=*this;
			if(mMouse!=nullptr)
				mMouse->resize(mRenderer.width(),mRenderer.height());
			for(auto&it:this->mCtrlList) {
				if(it!=nullptr&&it->visable()) {
					it->render();
					if(it->draw_by_scale()) {
						if(mMouse!=nullptr)
							mMouse->login({(std::size_t)it->posit()[0]*this->mWidth,(std::size_t)it->posit()[1]*this->mHeight}, {it->real_width(),it->real_height()},it);
						mRenderer.draw_by_scale(it->posit(),it->surface());
					} else {
						if(mMouse!=nullptr)
							mMouse->login({(std::size_t)it->posit()[0],(std::size_t)it->posit()[1]}, {it->real_width(),it->real_height()},it);
						mRenderer.draw({(std::size_t)it->posit()[0],(std::size_t)it->posit()[1]},it->surface());
					}
				}
			}
			this->mCtrlList.remove(nullptr);
			if(mMouse!=nullptr&&!this->mHideCursor)
				this->mRenderer.draw({mMouse->cursor_x(),mMouse->cursor_y()},this->mCursor);
		}
		void window::render()
		{
			this->mLocker.lock();
			this->mRenderer.resize(this->real_width(),this->real_height());
			for(std::size_t x=0; x<this->mRenderer.width(); ++x)
				this->mRenderer.draw({{x,0},{x,2},{x,this->mRenderer.height()-1}},this->mEdge);
			for(std::size_t y=0; y<this->mRenderer.height(); ++y)
				this->mRenderer.draw({{0,y},{this->mRenderer.width()-1,y}},this->mEdge);
			pixel pix(this->mEdge);
			for(std::size_t x(1); x<this->mRenderer.width()-1; ++x) {
				if(x-1<this->mTitle.size())
					pix.image(this->mTitle.at(x-1));
				else
					pix.image(' ');
				this->mRenderer.draw({x,1},pix);
			}
			mRenderer.draw({1,3},*this);
			for(auto&it:this->mCtrlList) {
				if(it!=nullptr&&it->visable()) {
					it->render();
					if(it->draw_by_scale())
						this->mRenderer.draw({(std::size_t)it->posit()[0]*this->mWidth+1,(std::size_t)it->posit()[1]*this->mHeight+3},it->surface());
					else
						this->mRenderer.draw({(std::size_t)it->posit()[0]+1,(std::size_t)it->posit()[1]+3},it->surface());
				}
			}
			this->mCtrlList.remove(nullptr);
			this->mFrameCache=this->mRenderer;
			this->mLocker.unlock();
		}
		void button::render()
		{
			this->mRenderer.resize(this->real_width(),this->real_height());
			this->mRenderer.fill(this->mEdge);
			pixel pix=this->mEdge;
			for(std::size_t x=1; x<mRenderer.width()-1; ++x) {
				if(x>1&&x<mRenderer.width()-2)
					pix.image(this->mText.at(x-2));
				else
					pix.image(' ');
				this->mRenderer.draw({x,1},pix);
			}
		}
		void menu::render()
		{
			this->mImg.resize(this->real_width(),this->real_height());
			this->mMouse.resize(this->mImg.width(),this->mImg.height());
			this->mAttri.image(' ');
			this->mImg.fill(this->mAttri);
			std::size_t count(0),begin(0);
			for(auto&it:this->mElements) {
				begin=(mImg.width()-it->text().size())*0.5;
				for(std::size_t x=begin; x-begin<it->text().size(); ++x) {
					it->attri().image(it->text().at(x-begin));
					this->mImg.draw({x,count},it->attri());
				}
				if(it->type()==typeid(menu)) {
					this->mAttri.image('>');
					this->mImg.draw({0,count},this->mAttri);
				}
				this->mMouse.login({0,count}, {this->mImg.width(),1},it);
				++count;
			}
		}
	}
}
#endif