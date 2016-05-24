#ifndef CGL_WINDOW_MAIN_H


#define CGL_WINDOW_MAIN_H


#include "../CGK/CGK2.h"
#include <functional>


namespace cgl
{
	static bool AutoWindowAdaptate = true;
	class window;
}


class cgl::window:public cgl::baseControl
{
  protected:
	baseView * object;
	int m_width, m_high;
	char m_border;
	c_color m_border_color;
	std::string m_title;
	std::function < void (window &) > draw_func;
	baseImage m_texture;
	bool df_available = false;
	void create()
	{
		m_border_color.set_surface(color::ground::fore);
		m_border_color = color::white;
		m_texture.alpha(false);
		ControlManager.add(this);
	}
  public:
	event onWindowShow;
	event onWindowHide;
	event onWindowFocus;
	event onWindowResize;
	event onBorderChange;
	event onTitleChange;
	event onBackChange;
	event onRenderDraw;
	window(int, int);
	window(baseView &);
	virtual ~ window()
	{
		ControlManager.remove(this);
	}
	virtual void title(const char *tit)
	{
		m_title = tit;
		onTitleChange.happen();
	}
	template < typename F > void set_draw_func(F & func)
	{
		draw_func = func;
		df_available = true;
	}
	virtual void border(char in)
	{
		m_border = in;
		onBorderChange.happen();
	}
	virtual char border()
	{
		return m_border;
	}
	virtual void border_color(char in)
	{
		m_border_color = in;
	}
	virtual void border_color(c_color & in)
	{
		m_border_color = in;
	}
	virtual c_color & border_color()
	{
		return m_border_color;
	}
	virtual void back(char in)
	{
		m_texture.back(in);
		m_render.back(in);
		onBackChange.happen();
	}
	virtual void render_back(char in)
	{
		m_render.alpha(false);
		m_render.back(in);
		onBackChange.happen();
	}
	virtual void back(c_color & in)
	{
		m_texture.back(in);
		m_render.back(in);
		onBackChange.happen();
	}
	virtual void render_back(c_color & in)
	{
		m_render.alpha(false);
		m_render.back(in);
		onBackChange.happen();
	}
	virtual c_color & back()
	{
		return m_texture.back();
	}
	virtual c_color & render_back()
	{
		return m_render.back();
	}
	virtual void focus()
	{
		ControlManager.focus(this);
		onWindowFocus.happen();
	}
	virtual void show()
	{
		m_visible = true;
		onWindowShow.happen();
	}
	virtual void hide()
	{
		m_visible = false;
		onWindowHide.happen();
	}
	virtual void reset()
	{
		m_render.reset();
	}
	virtual void resize(int w, int h)
	{
		m_width = w;
		m_high = h;
		int wid = m_width - 2;
		int hig = m_high - 4;
		m_render.set(wid, hig);
		m_texture.set(m_width, m_high);
		onWindowResize.happen();
	}
	virtual int width(bool real = false) const
	{
		if (real)
			return m_texture.width();
		else
			return m_render.width();
	}
	virtual int high(bool real = false) const
	{
		if (real)
			return m_texture.high();
		else
			return m_render.high();
	}
	baseImage & draw();
	virtual window & operator<<(baseControl & tmp)
	{
		m_render.draw(tmp.x, tmp.y, tmp.draw());
		return *this;
	}
	virtual point & read(int x, int y)
	{
		return m_render.read(x, y);
	}
	virtual bool draw(int x, int y, char ch, char attri = attribute::highlight)
	{
		return m_render.draw(x, y, ch, attri);
	}
	virtual bool draw(int x, int y, point & in)
	{
		return m_render.draw(x, y, in);
	}
	virtual bool draw(int x, int y, baseImage & in)
	{
		return m_render.draw(x, y, in);
	}
};


#endif