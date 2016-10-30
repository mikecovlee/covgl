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
// Core Version: 1.16.1

// Covariant Graphics Core

#include "covstdlib.hpp"
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
		class baseDevice;
		class baseWidget;
		class baseLayout;
		class baseActivity;
// 硬件控制器
		class baseMouse;
// 基础类
		class deviceEvent;
		class event;
		class pixel;
		class image;
// 硬件状态
		enum class device_states {
			null,ready,busy,sleep,error
		};
// 图像属性
		enum class attris {
			bright = 0xA0,
			underline = 0xA1
		};
// 图像颜色
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
// 鼠标动作
		enum class mouse_action {
			left_click,left_press,
			right_click,right_press,
			wheel_click,wheel_press,wheel_up,wheel_down
		};
// 基类
// 我们要求大部分CGL对象都派生于本基类
		class baseClass {
		protected:
			baseClass* mParent=nullptr;
		public:
			baseClass()=default;
			baseClass(const baseClass&)=default;
			baseClass(baseClass&&)=default;
			virtual ~baseClass()=default;
			// 设置父类指针
			virtual void setParent(baseClass* obj)
			{
				this->mParent=obj;
			}
			// 在基类中设计类型获取函数，便于识别对象类型
			virtual const std::type_info& type() const final
			{
				return typeid(*this);
			}
		};
// 设备基类
		class baseDevice:public baseClass {
		public:
			baseDevice()=default;
			baseDevice(const baseDevice&)=delete;
			baseDevice(baseDevice&&) noexcept=delete;
			virtual ~baseDevice()=default;
			virtual bool install()=0;
			virtual bool uninstall()=0;
			virtual bool wake()=0;
			virtual bool sleep()=0;
			virtual std::size_t getId() const=0;
			virtual device_states getState() const=0;
		};
// 鼠标基类
		class baseMouse:public baseDevice {
		public:
			baseMouse()=default;
			baseMouse(const baseMouse&)=delete;
			baseMouse(baseMouse&&) noexcept=delete;
			virtual ~baseMouse()=default;
			virtual std::size_t getX() const=0;
			virtual std::size_t getY() const=0;
			virtual std::size_t getRelativeX() const=0;
			virtual std::size_t getRelativeY() const=0;
			virtual mouse_action getAction() const=0;
		};
// 设备事件
		class deviceEvent {
		protected:
			std::deque<cov::function<bool(baseClass*,baseDevice*)>> mMethods;
		public:
			deviceEvent()=default;
			deviceEvent(const cov::function<bool(baseClass*,baseDevice*)>& f)
			{
				mMethods.push_front(f);
			}
			~deviceEvent()=default;
			void active(baseClass* ptr,baseDevice* dev) const
			{
				for(auto&it:mMethods) {
					if(it.callable()) {
						if(it(ptr,dev))
							break;
					}
				}
			}
			// 设置事件处理函数
			void set_listener(const cov::function<bool(baseClass*,baseDevice*)>& f)
			{
				mMethods.push_front(f);
			}
		};
// 事件类
// 你可以利用事件来灵活处理一些无法预期的行为
// 在相应的事件处理函数中你可以利用std::dynamic_cast将baseClass指针进行下行转换
		class event {
		protected:
			// 使用CovFunction支持各类函数
			// 要求必须接收一个baseClass*指针作为参数
			std::deque<cov::function<bool(baseClass*)>> mMethods;
		public:
			event()=default;
			event(const cov::function<bool(baseClass*)>& f)
			{
				mMethods.push_front(f);
			}
			~event()=default;
			// 使用一个基类指针激活事件
			void active(baseClass* ptr) const
			{
				for(auto&it:mMethods) {
					if(it.callable()) {
						if(it(ptr))
							break;
					}
				}
			}
			// 设置事件处理函数
			void set_listener(const cov::function<bool(baseClass*)>& f)
			{
				mMethods.push_front(f);
			}
		};
// 像素类
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
			// 多点绘制函数，可设置为连接各个点
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
			// 绘制图像函数
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
		};
// 控件基类
		class baseWidget:public baseClass {
		protected:
			bool mVisible=false;
		public:
			virtual bool onDraw() {};
			virtual bool onShow() {};
			virtual bool onHide() {};
			virtual bool onFocus() {};
			virtual bool onLost() {};
			virtual bool onMouse(baseDevice*)=0;
			static event onCreateEvent;
			static event onDestoryEvent;
			event onDrawEvent;
			event onShowEvent;
			event onHideEvent;
			event onFocusEvent;
			event onLostEvent;
			deviceEvent onMouseEvent;

			baseWidget()
			{
				onCreateEvent.active(this);
			}
			baseWidget(const baseWidget&)
			{
				onCreateEvent.active(this);
			}
			virtual ~baseWidget()
			{
				onDestoryEvent.active(this);
			}

			virtual void show()
			{
				this->mVisible=true;
				this->onShow();
				this->onShowEvent.active(this);
			}
			virtual void hide()
			{
				this->mVisible=false;
				this->onHide();
				this->onHideEvent.active(this);
			}
			virtual bool getVisible() const
			{
				return this->mVisible;
			}
			virtual std::size_t getX() const=0;
			virtual std::size_t getY() const=0;
			virtual std::size_t getWidth() const=0;
			virtual std::size_t getHeight() const=0;
			virtual const image& getImage()=0;
		};
// 排版基类
		class baseLayout:public baseClass {
		public:
			baseLayout()=default;
			baseLayout(const baseLayout&)=default;
			baseLayout(baseLayout&&) noexcept=default;
			virtual ~baseLayout()=default;
			virtual void onDraw(const image&)=0;
			virtual void addWidget(baseWidget*)=0;
			virtual void removeWidget(baseWidget*)=0;
		};
// 活动(Activity)基类
		class baseActivity:public baseClass {
		protected:
			// 焦点对象
			baseWidget* mFocalPoint=nullptr;
			// 控件列表
			std::list<baseWidget*> mCtrlList;
		public:
			baseActivity()=default;
			baseActivity(const baseActivity&)=default;
			baseActivity(baseActivity&&)=default;
			virtual ~baseActivity()=default;
			baseActivity& operator=(const baseActivity&)=default;
			baseActivity& operator=(baseActivity&&)=default;
			// 控件的注册与注销
			void login(baseWidget* ptr)
			{
				if(ptr==nullptr)
					throw;
				for(auto&it:this->mCtrlList) if(it==ptr) return;
				this->mCtrlList.push_front(ptr);
			}
			void logout(baseWidget* ptr)
			{
				if(ptr==nullptr)
					throw;
				this->mCtrlList.remove(ptr);
				if(this->mFocalPoint==ptr)
					this->mFocalPoint=nullptr;
			}
			bool registered(baseWidget* ptr) const
			{
				if(ptr==nullptr)
					throw;
				for(auto&it:this->mCtrlList) if(it==ptr) return true;
				return false;
			}
			// 聚焦控件
			void focus(baseWidget* ptr)
			{
				if(ptr==nullptr)
					throw;
				if(!this->registered(ptr))
					throw std::logic_error(__func__);
				this->logout(ptr);
				this->mCtrlList.push_back(ptr);
				this->mFocalPoint=ptr;
				ptr->onFocus();
				ptr->onFocusEvent.active(ptr);
			}
			// 前置控件
			void front(baseWidget* ptr)
			{
				if(ptr==nullptr)
					throw;
				if(!this->registered(ptr))
					throw std::logic_error(__func__);
				this->logout(ptr);
				this->mCtrlList.push_back(ptr);
			}
			// 设置及更换焦点
			void focal_point(baseWidget* ptr)
			{
				if(ptr==nullptr)
					throw;
				if(!this->registered(ptr))
					throw std::logic_error(__func__);
				this->mFocalPoint->onLost();
				this->mFocalPoint->onLostEvent.active(this->mFocalPoint);
				this->mFocalPoint=ptr;
				ptr->onFocus();
				ptr->onFocusEvent.active(ptr);
			}
			baseWidget* focal_point()
			{
				return mFocalPoint;
			}
			baseWidget* const focal_point() const
			{
				return mFocalPoint;
			}
			// Activity的渲染及绘制
			virtual const image& getImage() const=0;
			virtual void onDraw()=0;
		};
	}
}