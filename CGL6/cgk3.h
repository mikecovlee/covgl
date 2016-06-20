#pragma once

// Covariant Graphics Library

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
// Version 6.16.8.10 Beta

// Head File
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
			baseLambda* mFunc;
			static std::size_t eventId;
			std::size_t mId=++eventId;
		public:
			event()=delete;
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
				mFunc->active(ptr);
			}
		};
// 像素类
#ifdef CGL_CONSOLE
		class pixel {
		protected:
			// 属性
			std::array<bool,2> mAttri= {{false,false}};
			// 颜色
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
					delete[] mImage;
					mImage=new pixel[w*h];
					mWidth=w;
					mHeight=h;
					for(int i=0; i<mWidth*mHeight; ++i)
						mImage[i]=img[i];
				}
			}
		public:
			image():mWidth(0),mHeight(0),mImage(nullptr) {}
			image(std::size_t w,std::size_t h):mImage(new pixel[h*w]),mWidth(w),mHeight(h) {}
			image(std::size_t w,std::size_t h,const pixel& pix):mImage(new pixel[h*w]),mWidth(w),mHeight(h)
			{
				for(pixel* it=mImage; it!=mImage+w*h; ++it) *it=pix;
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
				delete[] mImage;
			}
			image& operator=(const image& img)
			{
				if(&img!=this)
					copy(img.mWidth,img.mHeight,img.mImage);
				return *this;
			}
			image& operator=(image&& img)
			{
				if(&img!=this)
					copy(img.mWidth,img.mHeight,img.mImage);
				return *this;
			}
			// Atrribute获取函数
			bool usable() const
			{
				return mImage!=nullptr;
			}
			std::size_t width() const
			{
				return mWidth;
			}
			std::size_t height() const
			{
				return mHeight;
			}
			// 重新设置尺寸
			void resize(std::size_t w,std::size_t h)
			{
				delete[] mImage;
				mImage=new pixel[h*w];
				mWidth=w;
				mHeight=h;
			}
			// 填充图像
			void fill(const pixel& pix)
			{
				if(mImage==nullptr)
					throw std::logic_error(__func__);
				for(pixel* it=mImage; it!=mImage+mWidth*mHeight; ++it) *it=pix;
			}
			// 清空图像
			void clear()
			{
				if(mImage!=nullptr) {
					delete[] mImage;
					mImage=new pixel[mHeight*mWidth];
				}
			}
			// 通过二维坐标访问图像
			pixel& at(const std::array<std::size_t,2>& posit)
			{
				if(mImage==nullptr)
					throw std::logic_error(__func__);
				if(posit[0]<0||posit[1]<0||posit[0]>mWidth-1||posit[1]>mHeight-1)
					throw std::out_of_range(__func__);
				return mImage[posit[1]*mWidth+posit[0]];
			}
			const pixel& at(const std::array<std::size_t,2>& posit) const
			{
				if(mImage==nullptr)
					throw std::logic_error(__func__);
				if(posit[0]<0||posit[1]<0||posit[0]>mWidth-1||posit[1]>mHeight-1)
					throw std::out_of_range(__func__);
				return mImage[posit[1]*mWidth+posit[0]];
			}
			// 单点绘制函数
			void draw(const std::array<double,2>& posit,const pixel& pix,bool draw_by_scale=false)
			{
				if(draw_by_scale)
					at({std::size_t(mWidth*posit[0]),std::size_t(mHeight*posit[1])})=pix;
				else
					at({std::size_t(posit[0]),std::size_t(posit[1])})=pix;
			}
			// 多点绘制函数，可设置为连接各个点
			void draw(const std::list<std::array<double,2>>& points,const pixel&pix,bool connect_points=false,bool draw_by_scale=false)
			{
				if(!connect_points) {
					for(auto &it:points)
						draw(it,pix,draw_by_scale);
				} else {
					if(draw_by_scale)
						throw std::logic_error(__func__);
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
							draw({x1+c*w,y1+c*h},pix);
						}
					}
				}
			}
			// 绘制图像函数
			void draw(const std::array<double,2>& posit,const image& img,bool draw_by_scale=false)
			{
				std::size_t col(posit[0]),row(posit[1]);
				if(draw_by_scale) {
					col=mWidth*posit[0];
					row=mHeight*posit[1];
				}
				if(mImage==nullptr)
					throw std::logic_error(__func__);
				if(col<0||row<0||col>mWidth-1||row>mHeight-1)
					throw std::out_of_range(__func__);
				for(std::size_t r=row; r<mHeight&&r-row<img.mHeight; ++r)
					for(std::size_t c=col; c<mWidth&&c-col<img.mWidth; ++c)
						mImage[r*mWidth+c]=img.mImage[(r-row)*img.mWidth+(c-col)];
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
			virtual ~baseActivity();
			// 控件的注册与注销
			void login(baseCtrl*);
			void logout(baseCtrl*);
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
				this->mCtrlList.push_front(ptr);
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
			virtual mouse* mouse_controller()=0;
			virtual mouse* const mouse_controller() const=0;
			virtual keyboard* keyboard_controller()=0;
			virtual keyboard* const keyboard_controller() const=0;
			virtual gamepad* gamepad_controller()=0;
			virtual gamepad* const gamepad_controller() const=0;
			virtual joystick* joystick_controller()=0;
			virtual joystick* const joystick_controller() const=0;
		};
// 控件(Control)基类
		class baseCtrl:public baseClass {
		protected:
			// 宿主Activity指针
			baseActivity* mHost=nullptr;
			// 可见性
			bool mVisable=false;
			// 是否通过比例绘制
			bool mDrawByScale=false;
			// 位置
			std::array<double,2> mPosit;
		public:
			static void mouse_event_handle(baseClass*) {}
			static void keyboard_event_handle(baseClass*) {}
			static void gamepad_event_handle(baseClass*) {}
			// 鼠标键盘以及游戏手柄事件，这里默认链接到了三个空函数上，同志们不要偷懒，最好自己写一个
			event mouse_event;
			event keyboard_event;
			event gamepad_event;
			// 构造函数
			baseCtrl():mPosit({0,0}),mouse_event(mouse_event_handle),keyboard_event(keyboard_event_handle),gamepad_event(gamepad_event_handle) {}
			baseCtrl(const baseCtrl&)=default;
			baseCtrl(baseCtrl&&)=default;
			virtual ~baseCtrl()
			{
				if(this->mHost!=nullptr)
					this->mHost->logout(this);
			}
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
			baseActivity* host()
			{
				return this->mHost;
			}
			baseActivity* const host() const
			{
				return this->mHost;
			}
			void host(baseActivity* ptr)
			{
				this->mHost=ptr;
			}
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
		};
// 各类的类外实现
		std::size_t event::eventId=0;
		baseActivity::~baseActivity()
		{
			for(auto&it:this->mCtrlList)
				it->host(nullptr);
		}
		void baseActivity::login(baseCtrl* ptr)
		{
			for(auto&it:this->mCtrlList) if(it==ptr) return;
			this->mCtrlList.push_back(ptr);
			ptr->host(this);
		}
		void baseActivity::logout(baseCtrl* ptr)
		{
			this->mCtrlList.remove(ptr);
			if(this->mFocalPoint==ptr)
				this->mFocalPoint=nullptr;
			ptr->host(nullptr);
		}
	}
}