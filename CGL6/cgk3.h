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
// Kernel Version: 3.16.7.11

#if __cplusplus < 201103L
#error Covariant C++ Library需要您的编译器支持C++11(C++0x)或者更高标准。请检查您否忘记了[-std=c++11]编译选项。
#else

// Covariant Graphics Kernel 3

#include <stdexcept>
#include <typeinfo>
#include <cmath>
#include <utility>
#include <array>
#include <list>

namespace cov {
	namespace gl {
// 全局基类
		class baseClass;
		class baseCtrl;
		class baseActivity;
// 硬件控制器
		class mouse;
		class keyboard;
		class gamepad;
		class joystick;
// 基础类
		class event;
		class pixel;
		class image;
#ifdef CGL_CONSOLE
// 控制台图像属性
		enum class attris {
		    bright = 0xA0,
		    underline = 0xA1
		};
// 控制台图像颜色
		enum class colors {
		    white = 0xC0,
		    black = 0xC1,
		    red = 0xC2,
		    green = 0xC3,
		    blue = 0xC4,
		    magenta = 0xC5,
		    yellow = 0xC6,
		    cyan = 0xC7
		};
#endif
// 基类
// 我们要求大部分CGL对象都派生于本基类
		class baseClass {
		public:
			baseClass()=default;
			baseClass(const baseClass&)=default;
			baseClass(baseClass&&)=default;
			virtual ~baseClass()=default;
			// 在基类中设计类型获取函数，便于识别对象类型
			virtual const std::type_info& type() const
			{
				return typeid(*this);
			}
		};
// 事件类
// 你可以利用事件来灵活处理一些无法预期的行为
// 在相应的事件处理函数中你可以利用std::dynamic_cast将baseClass指针进行下行转换
		class event {
		protected:
			// 通过抽象灵活支持各类函数，包括Lambda表达式
			// 要求必须接收一个baseClass*指针作为参数
			class baseLambda {
			public:
				baseLambda()=default;
				baseLambda(const baseLambda&)=default;
				baseLambda(baseLambda&&)=default;
				virtual ~baseLambda()=default;
				virtual void active(baseClass*) const=0;
			};
			template<typename T>class lambda:public baseLambda {
			protected:
				T mLambda;
			public:
				lambda()=delete;
				lambda(T func):mLambda(func) {}
				lambda(const lambda&)=default;
				lambda(lambda&&)=default;
				virtual ~lambda()=default;
				virtual void active(baseClass* ptr) const override
				{
					mLambda(ptr);
				}
			};
			baseLambda* mFunc=nullptr;
			static std::size_t eventId;
			std::size_t mId=++eventId;
		public:
			event()=default;
			template<typename T>event(T func):mFunc(new lambda<T>(func)) {}
			event(const event&)=delete;
			event(event&&)=delete;
			~event()
			{
				delete mFunc;
			}
			// 事件识别码的获取以及比较
			std::size_t operator~() const
			{
				return this->mId;
			}
			bool operator==(const event& ev)
			{
				return mId==~ev;
			}
			bool operator!=(const event& ev)
			{
				return mId!=~ev;
			}
			// 使用一个基类指针激活事件
			void active(baseClass* ptr) const
			{
				if(mFunc!=nullptr)
					mFunc->active(ptr);
			}
			// 重新设置事件处理函数
			template<typename T> void rebind(T func)
			{
				delete mFunc;
				mFunc=new lambda<T>(func);
			}
		};
// 像素类
#ifdef CGL_CONSOLE
		class pixel {
		protected:
			// 属性
			// 数组第一个元素为高亮属性，第二个为下划线属性
			std::array<bool,2> mAttri= {{false,false}};
			// 颜色
			// 数组第一个元素为前景色，第二个元素为背景色
			std::array<colors,2> mColor= {{colors::white,colors::black}};
			// 字符
			char mPix=' ';
		public:
			pixel()=default;
			pixel(const pixel&)=default;
			pixel(pixel&&)=default;
			pixel(const std::array<bool,2>&a,const std::array<colors,2>& c,char pix):mAttri(a),mColor(c),mPix(pix) {}
			~pixel()=default;
			pixel& operator=(const pixel&)=default;
			pixel& operator=(pixel&&)=default;
			// Attribute设置和获取函数
			std::array<bool,2>& attri()
			{
				return mAttri;
			}
			const std::array<bool,2>& attri() const
			{
				return mAttri;
			}
			void attri(attris a,bool key=true)
			{
				switch(a) {
				case attris::bright:
					mAttri[0]=key;
					break;
				case attris::underline:
					mAttri[1]=key;
					break;
				default:
					throw std::out_of_range(__func__);
				}
			}
			void clear_attri()
			{
				for(auto &it:mAttri)
					it=false;
			}
			// 颜色
			std::array<colors,2>& color()
			{
				return mColor;
			}
			const std::array<colors,2>& color() const
			{
				return mColor;
			}
			void color(const std::array<colors,2>& c)
			{
				mColor=c;
			}
			// 图像(字符)
			char image() const
			{
				return mPix;
			}
			// 如果设置的字符不是合法字符，那么会被替换为空格
			void image(char pix)
			{
				if (pix < 32 || pix > 126)
					mPix = ' ';
				else
					mPix=pix;
			}
		};
#else
		class pixel {
		protected:
			// RGBA格式32bit颜色
			std::array<unsigned char,4> mColor= {{0xff,0xff,0xff,0xff}};
		public:
			pixel()=default;
			pixel(const pixel&)=default;
			pixel(pixel&&)=default;
			pixel(const std::array<unsigned char,4>&c):mColor(c) {}
			~pixel()=default;
			pixel& operator=(const pixel&)=default;
			pixel& operator=(pixel&&)=default;
			unsigned char red() const
			{
				return mColor[0];
			}
			void red(unsigned char val)
			{
				mColor[0]=val;
			}
			unsigned char green() const
			{
				return mColor[1];
			}
			void green(unsigned char val)
			{
				mColor[1]=val;
			}
			unsigned char blue() const
			{
				return mColor[2];
			}
			void blue(unsigned char val)
			{
				mColor[2]=val;
			}
			unsigned char alpha() const
			{
				return mColor[3];
			}
			void alpha(unsigned char val)
			{
				mColor[3]=val;
			}
			std::array<unsigned char,4>& raw_data()
			{
				return mColor;
			}
			const std::array<unsigned char,4>& raw_data() const
			{
				return mColor;
			}
		};
#endif
// 图像类
		class image {
		protected:
			// 尺寸
			std::size_t mWidth,mHeight;
			// 源数据
			pixel* mImage=nullptr;
			// 拷贝函数
			void copy(std::size_t w,std::size_t h,pixel* const img)
			{
				if(img!=nullptr) {
					delete[] this->mImage;
					this->mImage=new pixel[w*h];
					this->mWidth=w;
					this->mHeight=h;
					for(int i=0; i<this->mWidth*this->mHeight; ++i)
						this->mImage[i]=img[i];
				}
			}
		public:
			image():mWidth(0),mHeight(0),mImage(nullptr) {}
			image(std::size_t w,std::size_t h):mImage(new pixel[h*w]),mWidth(w),mHeight(h) {}
			image(std::size_t w,std::size_t h,const pixel& pix):mImage(new pixel[h*w]),mWidth(w),mHeight(h)
			{
				for(pixel* it=this->mImage; it!=this->mImage+w*h; ++it) *it=pix;
			}
			image(const image& img):mWidth(0),mHeight(0),mImage(nullptr)
			{
				copy(img.mWidth,img.mHeight,img.mImage);
			}
			image(image&& img):mWidth(0),mHeight(0),mImage(nullptr)
			{
				copy(img.mWidth,img.mHeight,img.mImage);
			}
			~image()
			{
				delete[] this->mImage;
			}
			image& operator=(const image& img)
			{
				if(&img!=this)
					this->copy(img.mWidth,img.mHeight,img.mImage);
				return *this;
			}
			image& operator=(image&& img)
			{
				if(&img!=this)
					this->copy(img.mWidth,img.mHeight,img.mImage);
				return *this;
			}
			// Atrribute获取函数
			bool usable() const
			{
				return this->mImage!=nullptr;
			}
			std::size_t width() const
			{
				return this->mWidth;
			}
			std::size_t height() const
			{
				return this->mHeight;
			}
			// 重新设置尺寸
			// resize将令您失去所有数据，请注意备份
			// resize之前请注意判断您设置的尺寸是否与原来相同，以避免多余的性能开销
			void resize(std::size_t w,std::size_t h)
			{
				delete[] this->mImage;
				this->mImage=new pixel[h*w];
				this->mWidth=w;
				this->mHeight=h;
			}
			// 填充图像
			void fill(const pixel& pix)
			{
				if(this->mImage==nullptr)
					throw std::logic_error(__func__);
				for(pixel* it=this->mImage; it!=this->mImage+this->mWidth*this->mHeight; ++it) *it=pix;
			}
			// 清空图像
			void clear()
			{
				if(this->mImage!=nullptr) {
					delete[] this->mImage;
					this->mImage=new pixel[mHeight*mWidth];
				}
			}
			// 通过二维坐标访问图像
			// 您不必在意double与std::size_t之间转换的警告
			pixel& at(const std::array<std::size_t,2>& posit)
			{
				if(this->mImage==nullptr)
					throw std::logic_error(__func__);
				if(posit[0]<0||posit[1]<0||posit[0]>this->mWidth-1||posit[1]>this->mHeight-1)
					throw std::out_of_range(__func__);
				return this->mImage[posit[1]*this->mWidth+posit[0]];
			}
			const pixel& at(const std::array<std::size_t,2>& posit) const
			{
				if(this->mImage==nullptr)
					throw std::logic_error(__func__);
				if(posit[0]<0||posit[1]<0||posit[0]>this->mWidth-1||posit[1]>this->mHeight-1)
					throw std::out_of_range(__func__);
				return this->mImage[posit[1]*this->mWidth+posit[0]];
			}
			// 单点绘制函数
			void draw(const std::array<std::size_t,2>& posit,const pixel& pix)
			{
				this->at(posit)=pix;
			}
			void draw_by_scale(const std::array<double,2>& posit,const pixel& pix)
			{
				this->at({std::size_t(this->mWidth*posit[0]),std::size_t(this->mHeight*posit[1])})=pix;
			}
			// 多点绘制函数，可设置为连接各个点。当连接多个点时将不允许通过比例绘制
			void draw(const std::list<std::array<std::size_t,2>>& points,const pixel& pix,bool connect_points=false)
			{
				if(!connect_points) {
					for(auto &it:points)
						this->draw(it,pix);
				} else {
					auto p0=points.begin();
					auto p1=++points.begin();
					for(; p1!=points.end(); ++p0,++p1) {
						double x1(p0->at(0)),x2(p1->at(0)),y1(p0->at(1)),y2(p1->at(1)),w(x2-x1),h(y2-y1),a(0);
						if(w>=h)
							a=w;
						else
							a=h;
						a=std::abs(a);
						for(double c=0; c<=1; c+=1.0/a) {
							this->draw({std::size_t(x1+c*w),std::size_t(y1+c*h)},pix);
						}
					}
				}
			}
			void draw_by_scale(const std::list<std::array<double,2>>& points,const pixel&pix)
			{
				for(auto &it:points)
					this->draw_by_scale(it,pix);
			}
			// 绘制图像函数，你同样可以通过设置最后一个参数来开启通过比例绘制
			void draw(const std::array<std::size_t,2>& posit,const image& img)
			{
				std::size_t col(posit[0]),row(posit[1]);
				if(this->mImage==nullptr)
					throw std::logic_error(__func__);
				if(col<0||row<0||col>this->mWidth-1||row>this->mHeight-1)
					throw std::out_of_range(__func__);
				for(std::size_t r=row; r<this->mHeight&&r-row<img.mHeight; ++r)
					for(std::size_t c=col; c<this->mWidth&&c-col<img.mWidth; ++c)
						this->mImage[r*this->mWidth+c]=img.mImage[(r-row)*img.mWidth+(c-col)];
			}
			void draw_by_scale(const std::array<double,2>& posit,const image& img,bool draw_by_scale=false)
			{
				std::size_t col(this->mWidth*posit[0]),row(this->mHeight*posit[1]);
				if(this->mImage==nullptr)
					throw std::logic_error(__func__);
				if(col<0||row<0||col>this->mWidth-1||row>this->mHeight-1)
					throw std::out_of_range(__func__);
				for(std::size_t r=row; r<this->mHeight&&r-row<img.mHeight; ++r)
					for(std::size_t c=col; c<this->mWidth&&c-col<img.mWidth; ++c)
						this->mImage[r*this->mWidth+c]=img.mImage[(r-row)*img.mWidth+(c-col)];
			}
		};
// 活动(Activity)基类
		class baseActivity:public baseClass {
		protected:
			// 焦点对象
			baseCtrl* mFocalPoint=nullptr;
			// 控件列表
			std::list<baseCtrl*> mCtrlList;
		public:
			baseActivity()=default;
			baseActivity(const baseActivity&)=default;
			baseActivity(baseActivity&&)=default;
			virtual ~baseActivity()=default;
			baseActivity& operator=(const baseActivity&)=default;
			baseActivity& operator=(baseActivity&&)=default;
			// 控件的注册与注销
			void login(baseCtrl* ptr)
			{
				for(auto&it:this->mCtrlList) if(it==ptr) return;
				this->mCtrlList.push_front(ptr);
			}
			void logout(baseCtrl* ptr)
			{
				this->mCtrlList.remove(ptr);
				if(this->mFocalPoint==ptr)
					this->mFocalPoint=nullptr;
			}
			bool registered(baseCtrl* ptr) const
			{
				for(auto&it:this->mCtrlList) if(it==ptr) return true;
				return false;
			}
			// 聚焦控件
			void focus(baseCtrl* ptr)
			{
				if(!this->registered(ptr))
					throw std::logic_error(__func__);
				this->logout(ptr);
				this->mCtrlList.push_back(ptr);
				this->mFocalPoint=ptr;
			}
			// 设置及更换焦点
			void focal_point(baseCtrl* ptr)
			{
				if(!this->registered(ptr))
					throw std::logic_error(__func__);
				this->mFocalPoint=ptr;
			}
			baseCtrl* focal_point()
			{
				return mFocalPoint;
			}
			baseCtrl* const focal_point() const
			{
				return mFocalPoint;
			}
			// Activity的渲染及绘制
			virtual const image& surface() const=0;
			virtual void render()=0;
			// Activity的硬件控制器
			virtual mouse* mouse_controller()
			{
				return nullptr;
			}
			virtual mouse* const mouse_controller() const
			{
				return nullptr;
			}
			virtual keyboard* keyboard_controller()
			{
				return nullptr;
			}
			virtual keyboard* const keyboard_controller() const
			{
				return nullptr;
			}
			virtual gamepad* gamepad_controller()
			{
				return nullptr;
			}
			virtual gamepad* const gamepad_controller() const
			{
				return nullptr;
			}
			virtual joystick* joystick_controller()
			{
				return nullptr;
			}
			virtual joystick* const joystick_controller() const
			{
				return nullptr;
			}
		};
// 控件(Control)基类
		class baseCtrl:public baseClass {
		protected:
			// 可见性
			bool mVisable=true;
			// 是否通过比例绘制
			bool mDrawByScale=false;
			// 位置
			std::array<double,2> mPosit;
		public:
			// 鼠标键盘以及游戏手柄事件
			event mouse_event;
			event keyboard_event;
			event gamepad_event;
			// 构造函数
			baseCtrl():mPosit({0,0}) {}
			baseCtrl(const baseCtrl&)=default;
			baseCtrl(baseCtrl&&)=default;
			virtual ~baseCtrl()=default;
			// 做了好久的思想斗争，最终还是把show和hide改成了虚函数，同志们尽情DIY吧…
			virtual void show()
			{
				this->mVisable=true;
			}
			virtual void hide()
			{
				this->mVisable=false;
			}
			// 这些基本的Attribute设置获取函数就算了吧，性能优先
			bool visable() const
			{
				return this->mVisable;
			}
			bool draw_by_scale() const
			{
				return this->mDrawByScale;
			}
			void draw_by_scale(bool set)
			{
				this->mDrawByScale=set;
			}
			std::array<double,2>& posit()
			{
				return this->mPosit;
			}
			const std::array<double,2>& posit() const
			{
				return this->mPosit;
			}
			void posit(const std::array<double,2>& p)
			{
				this->mPosit=p;
			}
			void posit(std::array<double,2>&& p)
			{
				this->mPosit=p;
			}
			// 控件的渲染及绘制
			virtual const image& surface() const=0;
			virtual void render()=0;
			// 控件实际尺寸的获取
			virtual std::size_t real_width() const=0;
			virtual std::size_t real_height() const=0;
		};
		std::size_t event::eventId=0;
	}
}
#endif