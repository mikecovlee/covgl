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
// Version: 6.16.8.10 Beta

#if __cplusplus < 201103L
#error Covariant C++ Library需要您的编译器支持C++11(C++0x)或者更高标准。请检查您否忘记了[-std=c++11]编译选项。
#else

// CGL主文件

#include "cgk3.h"
#include "basic_io.h"
#include <vector>
#include <thread>
#include <mutex>
#include <atomic>

namespace cov {
	namespace gl {
		class screen:virtual public baseActivity,virtual public image {
		protected:
			mouse* mMouse=nullptr;
			keyboard* mKeyboard=nullptr;
			gamepad* mGamepad=nullptr;
			joystick* mJoystick=nullptr;
			std::thread mDrawThread;
			std::atomic<bool> mThreadRunning;
			mutable std::mutex mThreadLocker;
			std::vector<image> mFrameCache;
		public:
			screen();
			screen(const screen&)=delete;
			screen(screen&&)=delete;
			screen& operator=(const screen&)=delete;
			screen& operator=(screen&&)=delete;
			void startDrawThread()
			{
				mThreadRunning=true;
			}
			void stopDrawThread()
			{
				mThreadRunning=false;
			}
			virtual const image& surface() const override
			{
				mThreadLocker.lock();
				if(mFrameCache.empty()) {
					mThreadLocker.unlock();
					return *this;
				} else {
					const image& ret=mFrameCache.back();
					mThreadLocker.unlock();
					return ret;
				}
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
		screen::screen():mDrawThread([&] {
			while(true)
			{
				this->mThreadLocker.lock();
				if(this->mThreadRunning&&!this->mFrameCache.empty()) {
					ioctrl.update_image(this->mFrameCache.back());
					this->mFrameCache.pop_back();
				} else {
					std::this_thread::yield();
				}
				this->mThreadLocker.unlock();
			}
		}),mThreadRunning(false)
		{
			mDrawThread.detach();
		}
		void screen::render()
		{
			this->mThreadLocker.lock();
			image renderer(*this);
			for(auto&it:this->mCtrlList) {
				if(it!=nullptr) {
					it->render();
					if(it->draw_by_scale())
						renderer.draw_by_scale(it->posit(),it->surface());
					else
						renderer.draw({(std::size_t)it->posit()[0],(std::size_t)it->posit()[1]},it->surface());
				}
			}
			this->mCtrlList.remove(nullptr);
			this->mFrameCache.push_back(renderer);
			this->mThreadLocker.unlock();
		}
	}
}

#endif