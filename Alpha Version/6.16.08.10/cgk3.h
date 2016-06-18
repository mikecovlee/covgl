// Covariant Graphics Library
// Version 6.16.8.10 Alpha
// Head File
#include <stdexcept>
#include <typeinfo>
#include <cmath>
#include <deque>
#include <utility>
#include <array>
#include <list>
namespace cov {
	namespace gl {
		class baseClass {
		public:
			baseClass()=default;
			baseClass(const baseClass&)=default;
			baseClass(baseClass&&)=default;
			virtual ~baseClass()=default;
			virtual const std::type_info& type() const
			{
				return typeid(*this);
			}
		};
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
		std::size_t event::eventId=0;
		enum class attris {
		    alpha = 0xA0,
		    bright = 0xA1,
		    underline = 0xA2
		};
		enum class layers {
		    fore = 0xB0,
		    back = 0xB1
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
		class pixel {
		protected:
			std::array<bool,3> mAttri= {{false,false,false}};
			layers mLayer=layers::back;
			colors mColor=colors::white;
			char mPix=' ';
		public:
			pixel()=default;
			pixel(const pixel&)=default;
			pixel(pixel&&)=default;
			pixel(const std::array<bool,3>&a,layers l,colors c,char pix):mAttri(a),mLayer(l),mColor(c),mPix(pix) {}
			~pixel()=default;
			pixel& operator=(const pixel&)=default;
			pixel& operator=(pixel&&)=default;
			// Attribute设置和获取函数
			const std::array<bool,3>& attri() const
			{
				return mAttri;
			}
			void attri(attris a,bool key=true)
			{
				switch(a) {
				case attris::alpha:
					mAttri[0]=key;
					break;
				case attris::bright:
					mAttri[1]=key;
					break;
				case attris::underline:
					mAttri[2]=key;
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
			// 图层
			layers layer() const
			{
				return mLayer;
			}
			void layer(layers lay)
			{
				mLayer=lay;
			}
			// 颜色
			colors color() const
			{
				return mColor;
			}
			void color(colors c)
			{
				mColor=c;
			}
			// 图像
			char image() const
			{
				return mPix;
			}
			void image(char pix)
			{
				mPix=pix;
			}
		};
		class image {
		protected:
			std::size_t mWidth,mHeight;
			pixel* mImage=nullptr;
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
				if(posit[0]<0||posit[1]<0||posit[0]>mWidth||posit[1]>mHeight)
					throw std::out_of_range(__func__);
				return mImage[posit[1]*mWidth+posit[0]];
			}
			const pixel& at(const std::array<std::size_t,2>& posit) const
			{
				if(mImage==nullptr)
					throw std::logic_error(__func__);
				if(posit[0]<0||posit[1]<0||posit[0]>mWidth||posit[1]>mHeight)
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
			void draw(const std::deque<std::array<double,2>>& points,const pixel&pix,bool connect_points=false,bool draw_by_scale=false)
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
						for(double c=0; c<=1; c+=1/a) {
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
				if(col<0||row<0||col>mWidth||row>mHeight)
					throw std::out_of_range(__func__);
				for(std::size_t r=row; r<mHeight&&r-row<img.mHeight; ++r)
					for(std::size_t c=col; c<mWidth&&c-col<img.mWidth; ++c)
						mImage[r*mWidth+c]=img.mImage[(r-row)*img.mWidth+(c-col)];
			}
		};
		class baseCtrl;
		class cursor_map {
		protected:
			baseCtrl** mCMap;
			std::size_t mWidth,mHeight,mCX,mCY;
			void copy(std::size_t w,std::size_t h,baseCtrl** const map)
			{
				if(map!=nullptr) {
					delete[] mCMap;
					mCMap=new baseCtrl*[w*h];
					mWidth=w;
					mHeight=h;
					for(int i=0; i<w*h; ++i)
						mCMap[i]=map[i];
				}
			}
		public:
			cursor_map():mCMap(nullptr),mWidth(0),mHeight(0),mCX(0),mCY(0) {}
			cursor_map(std::size_t w,std::size_t h):mCMap(new baseCtrl*[w*h]),mWidth(w),mHeight(h),mCX(0),mCY(0) {}
			cursor_map(const cursor_map& map):mCMap(nullptr),mWidth(0),mHeight(0),mCX(0),mCY(0)
			{
				copy(map.mWidth,map.mHeight,map.mCMap);
			}
			cursor_map(cursor_map&& map):mCMap(nullptr),mWidth(0),mHeight(0),mCX(0),mCY(0)
			{
				copy(map.mWidth,map.mHeight,map.mCMap);
			}
			~cursor_map()
			{
				delete[] mCMap;
			}
			cursor_map& operator=(const cursor_map& map)
			{
				if(&map!=this)
					copy(map.mWidth,map.mHeight,map.mCMap);
				return *this;
			}
			cursor_map& operator=(cursor_map&& map)
			{
				if(&map!=this)
					copy(map.mWidth,map.mHeight,map.mCMap);
				return *this;
			}
			void resize(std::size_t w,std::size_t h)
			{
				delete[] mCMap;
				mCMap=new baseCtrl*[w*h];
				mWidth=w;
				mHeight=h;
			}
			void clear()
			{
				if(mCMap!=nullptr) {
					delete[] mCMap;
					mCMap=new baseCtrl*[mWidth*mHeight];
				}
			}
			void login(const std::array<std::size_t,2>& posit,const std::array<std::size_t,2>& size,baseCtrl* ptr)
			{
				std::size_t col(posit[0]),row(posit[1]),sw(size[0]),sh(size[1]);
				if(mCMap==nullptr)
					throw std::logic_error(__func__);
				if(col<0||row<0||col>mWidth||row>mHeight)
					throw std::out_of_range(__func__);
				for(std::size_t r=row; r<mHeight&&r-row<sh; ++r)
					for(std::size_t c=col; c<mWidth&&c-col<sw; ++c)
						mCMap[r*mWidth+c]=ptr;
			}
			void login_with_image(const std::array<std::size_t,2>& posit,const image& img,baseCtrl* ptr)
			{
				login(posit, {img.width(),img.height()},ptr);
			}
			void logout(baseCtrl* ptr)
			{
				for(std::size_t i=0; i<mWidth*mHeight; ++i)
					if(mCMap[i]==ptr) mCMap=nullptr;
			}
			void active(const std::array<std::size_t,2>&);
		};
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
			const baseCtrl* focal_point() const
			{
				return mFocalPoint;
			}
			// Activity的渲染及绘制
			virtual const image& surface() const=0;
			virtual void render()=0;
		};
		class baseCtrl:public baseClass {
		protected:
			baseActivity* mHost=nullptr;
			bool mVisable=false;
			bool mDrawByScale=false;
			std::array<double,2> mPosit;
			image mImg;
		public:
			static void on_mouse_clicked(baseClass*) {}
			static void on_keyboard_pressed(baseClass*) {}
			// 鼠标键盘键入事件，这里默认链接到了两个空函数上，同志们不要偷懒，最好自己写一个
			event mouse_clicked;
			event keyboard_pressed;
			// 构造函数
			baseCtrl():mPosit({0,0}),mouse_clicked(on_mouse_clicked),keyboard_pressed(on_keyboard_pressed) {}
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
			baseActivity* host()
			{
				return this->mHost;
			}
			const baseActivity* host() const
			{
				return this->mHost;
			}
			void host(baseActivity* ptr)
			{
				if(this->mHost!=nullptr)
					this->mHost->logout(this);
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
			const image& surface() const
			{
				return this->mImg;
			}
			// 渲染函数直接设置为纯虚函数，同志们必须自己动手写咯
			virtual void render()=0;
		};
		void cursor_map::active(const std::array<std::size_t,2>& posit)
		{
			if(mCMap==nullptr)
				throw std::logic_error(__func__);
			if(posit[0]<0||posit[1]<0||posit[0]>mWidth||posit[1]>mHeight)
				throw std::out_of_range(__func__);
			baseCtrl* obj=mCMap[posit[1]*mWidth+posit[0]];
			obj->mouse_clicked.active(obj);
		}
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