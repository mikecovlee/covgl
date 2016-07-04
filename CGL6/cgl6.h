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

namespace cov {
	namespace gl {
// 屏幕类
		class screen:virtual public baseActivity,virtual public image {
		protected:
			mouse* mMouse=nullptr;
			keyboard* mKeyboard=nullptr;
			gamepad* mGamepad=nullptr;
			joystick* mJoystick=nullptr;
			basic_io* mIoctrl=nullptr;
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
			virtual const image& surface() const override
			{
				return mRenderer;
			}
			virtual void render() override;
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
		};
// Window控件类
		class window:public baseCtrl,public baseActivity,public image {
		protected:
			pixel mEdge;
			std::string mTitle;
			image mRenderer;
		public:
			window()=default;
			window(const window&)=delete;
			window(window&&)=delete;
			window& operator=(const window&)=delete;
			window& operator=(window&&)=delete;
			virtual ~window()=default;
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
				return mRenderer;
			}
			virtual void render() override;
			virtual mouse* mouse_controller() override
			{
				if(this->mHost!=nullptr)
					return this->mHost->mouse_controller();
				else
					return nullptr;
			}
			virtual mouse* const mouse_controller() const override
			{
				if(this->mHost!=nullptr)
					return this->mHost->mouse_controller();
				else
					return nullptr;
			}
			virtual keyboard* keyboard_controller() override
			{
				if(this->mHost!=nullptr)
					return this->mHost->keyboard_controller();
				else
					return nullptr;
			}
			virtual keyboard* const keyboard_controller() const override
			{
				if(this->mHost!=nullptr)
					return this->mHost->keyboard_controller();
				else
					return nullptr;
			}
			virtual gamepad* gamepad_controller() override
			{
				if(this->mHost!=nullptr)
					return this->mHost->gamepad_controller();
				else
					return nullptr;
			}
			virtual gamepad* const gamepad_controller() const override
			{
				if(this->mHost!=nullptr)
					return this->mHost->gamepad_controller();
				else
					return nullptr;
			}
			virtual joystick* joystick_controller() override
			{
				if(this->mHost!=nullptr)
					return this->mHost->joystick_controller();
				else
					return nullptr;
			}
			virtual joystick* const joystick_controller() const override
			{
				if(this->mHost!=nullptr)
					return this->mHost->joystick_controller();
				else
					return nullptr;
			}
		};
// 对象定义和类外实现
		static screen scr(&ioctrl);
		void screen::render()
		{
			mRenderer=*this;
			if(mMouse!=nullptr)
				mMouse->resize(mRenderer.width(),mRenderer.height());
			for(auto&it:this->mCtrlList) {
				if(it!=nullptr) {
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
			if(mMouse!=nullptr)
				this->mRenderer.draw({mMouse->cursor_x(),mMouse->cursor_y()},this->mCursor);
		}
		void window::render()
		{
			this->mRenderer.resize(this->real_width(),this->real_height());
			//this->mRenderer.fill(this->mEdge);
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
				if(it!=nullptr) {
					it->render();
					if(it->draw_by_scale())
						this->mRenderer.draw({(std::size_t)it->posit()[0]*this->mWidth+1,(std::size_t)it->posit()[1]*this->mHeight+3},it->surface());
					else
						this->mRenderer.draw({(std::size_t)it->posit()[0]+1,(std::size_t)it->posit()[1]+3},it->surface());
				}
			}
			this->mCtrlList.remove(nullptr);
		}
	}
}
#endif