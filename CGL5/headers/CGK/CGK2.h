#ifndef CGL_CGK2_H


#define CGL_CGK2_H


#include "Kernel.h"


namespace cgl
{
	// 字符属性代码:无，高亮(粗体)，下划线。(非必须：闪烁，反色，消隐)
	namespace attribute
	{
		static const char none = '0';
		static const char highlight = '1';
		static const char underline = '4';
		static const char flashing = '5';
		static const char reverse = '7';
		static const char blanking = '8';
	}
	// 字符颜色代码:红，绿，黄，蓝，粉，青，黑，白
	namespace color
	{
		// 图层代码:前景色或背景色
		namespace ground
		{
			static const char fore = '3';
			static const char back = '4';
		}
		static const char black = '0';
		static const char red = '1';
		static const char green = '2';
		static const char yellow = '3';
		static const char blue = '4';
		static const char magenta = '5';
		static const char cyan = '6';
		static const char white = '7';
	}
	// CGK版本
	const char *cgk_version = "2.2.1";
	// CGK函数和类型定义
	inline int get_terminal_width();
	inline int get_terminal_high();
	inline void reset_character_attri();
	inline void set_character_attri(char *);
	  template < typename > class c_image;
	class c_color;
	class position;
	class point;
	class basic;
	class baseImage;
	class baseView;
}


template < typename T > class cgl::c_image:public baseClass
{
	std::vector < std::vector < T >> data;
	int wid, hig;
	inline void resize(int, int);
  public:
	inline int width() const
	{
		return wid;
	}
	inline int high() const
	{
		return hig;
	}
#ifdef CGL_TRACK
	track::message out_of_range;
  c_image():wid(0), hig(0), out_of_range("out of range.", track::rank::error)
	{
	}
#else
	c_image():wid(0), hig(0)
	{
	}
#endif
	c_image(const c_image &);
	inline void set(int, int);
	inline c_image & operator=(const c_image &);
	inline T & read(int, int);
	inline void write(int, int, T);
	inline std::vector < std::vector < T >> &array()
	{
		return data;
	}
};


class cgl::c_color:public baseClass
{
	char m_surface, m_color;
	char m_mixed[3];
  public:
	event onColorChange;
	event onSurfaceSet;
	c_color():m_surface(color::ground::fore), m_color(color::white)
	{
	}
	inline void set_surface(char in)
	{
		m_surface = in;
		onSurfaceSet.happen();
	}
	inline c_color & operator=(const c_color & in)
	{
		m_color = in.m_color;
		onColorChange.happen();
		return *this;
	}
	inline char operator=(char in)
	{
		m_color = in;
		onColorChange.happen();
		return m_color;
	}
	inline operator        const char *()
	{
		m_mixed[0] = m_surface;
		m_mixed[1] = m_color;
		m_mixed[2] = '\0';
		return m_mixed;
	}
	inline const char *data()
	{
		m_mixed[0] = m_surface;
		m_mixed[1] = m_color;
		m_mixed[2] = '\0';
		return m_mixed;
	}
};


class cgl::position:public baseClass
{
	int m_position;
  public:
	event onPositionMove;
	position():m_position(0)
	{
	}
	inline int operator=(int in)
	{
		m_position = in;
		onPositionMove.happen();
		return in;
	}
	inline operator        int () const
	{
		return m_position;
	}
	inline int data() const
	{
		return m_position;
	}
	inline int operator++();
	inline int operator--();
};


class cgl::point
{
	char img;
	bool m_alpha = false;
	char m_attri = attribute::none;
  public:
	c_color text, back;
	point():img(' ')
	{
		text.set_surface(color::ground::fore);
		text = color::white;
		back.set_surface(color::ground::back);
		back = color::black;
	}
	point(char in):img(in)
	{
		text.set_surface(color::ground::fore);
		text = color::white;
		back.set_surface(color::ground::back);
		back = color::black;
	}
	bool alpha()
	{
		return m_alpha;
	}
	void alpha(bool alp)
	{
		m_alpha = alp;
	}
	char attribute()
	{
		return m_attri;
	}
	void attribute(char att)
	{
		m_attri = att;
	}
	inline char operator=(char tmp)
	{
		img = tmp;
		return img;
	}
	inline operator      char () const
	{
		return img;
	}
	inline char data() const
	{
		return img;
	}
};


class cgl::baseImage:public baseClass
{
  protected:
	bool m_use = false;
	bool m_blank = true;
	bool m_alpha = true;
	c_image < point > tmp_image;
	c_image < point > m_image;
	c_color m_back;
	inline c_image < point > &remix();
  public:
#ifdef CGL_TRACK
	track::message use_without_init;
	baseImage():use_without_init("use without init", track::rank::error)
	{
		m_back.set_surface(color::ground::back);
		m_back = color::black;
	}
#else
	baseImage()
	{
		m_back.set_surface(color::ground::back);
		m_back = color::black;
	}
#endif
	inline virtual void reset();
	inline virtual void set(int, int);
	inline virtual void alpha(bool in)
	{
		m_alpha = in;
	}
	inline virtual bool alpha() const
	{
		return m_alpha;
	}
	inline virtual void back(char in)
	{
		m_back = in;
	}
	inline virtual void back(c_color & in)
	{
		m_back = in;
	}
	inline virtual c_color & back()
	{
		return m_back;
	}
	inline virtual c_image < point > &image()
	{
		return remix();
	}
	inline virtual point & read(int, int);
	inline virtual bool draw(int, int, char, char);
	inline virtual bool draw(int, int, point &);
	inline virtual bool draw(int, int, baseImage &);
	inline virtual bool use() const
	{
		return m_use;
	}
	inline virtual int width() const
	{
		return m_image.width();
	}
	inline virtual int high() const
	{
		return m_image.high();
	}
};


class cgl::baseControl:public baseClass
{
  protected:
	bool m_visible = false;
	baseImage m_render;
  public:
	event onCtrlSelected;
	position x, y;
	baseControl();
	baseControl(int, int);
	virtual ~ baseControl()
	{
		ControlManager.remove(this);
	}
	virtual bool visible() const
	{
		return m_visible;
	}
	virtual void show()
	{
		m_visible = true;
	}
	virtual void hide()
	{
		m_visible = false;
	}
	virtual void focus()
	{
		ControlManager.focus(this);
	}
	virtual void select()
	{
		ControlManager.select(this);
	}
	virtual bool selected()
	{
		return ControlManager.selected(this);
	}
	virtual baseImage & draw()
	{
		return m_render;
	}
	virtual bool use()
	{
		return m_render.use();
	}
};


class cgl::baseView:public baseClass
{
	baseImage m_render;
  public:
	event onCtrlDraw;
	event onBackChange;
	event onRenderDraw;
	event onViewAdaptate;
	baseView()
	{
		m_render.set(get_terminal_width(), get_terminal_high());
		m_render.alpha(false);
	}
	virtual baseView & operator<<(baseControl & tmp)
	{
		onCtrlDraw.happen();
		m_render.draw(tmp.x, tmp.y, tmp.draw());
		return *this;
	}
	virtual int width() const
	{
		return m_render.width();
	}
	virtual int high() const
	{
		return m_render.high();
	}
	virtual void adaptate()
	{
		onViewAdaptate.happen();
		m_render.set(get_terminal_width(), get_terminal_high());
	}
	virtual void reset()
	{
		m_render.reset();
	}
	virtual void back(char in)
	{
		onBackChange.happen();
		m_render.back(in);
	}
	virtual void back(c_color & in)
	{
		onBackChange.happen();
		m_render.back(in);
	}
	virtual c_color & back()
	{
		return m_render.back();
	}
	virtual c_image < point > &image()
	{
		return m_render.image();
	}
	virtual point & read(int x, int y)
	{
		return m_render.read(x, y);
	}
	virtual bool draw(int x, int y, char ch, char attri = attribute::none)
	{
		onRenderDraw.happen();
		return m_render.draw(x, y, ch, attri);
	}
	virtual bool draw(int x, int y, point & in)
	{
		onRenderDraw.happen();
		return m_render.draw(x, y, in);
	}
	virtual bool draw(int x, int y, baseImage & in)
	{
		onRenderDraw.happen();
		return m_render.draw(x, y, in);
	}
};


#endif