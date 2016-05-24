#ifndef CGL_CONTROLS_H


#define CGL_CONTROLS_H


#include "../CGK/CGK2.h"


namespace cgl
{
	class c_text;
	class button;
	typedef c_text text;
}


class cgl::c_text:public baseControl
{
  protected:
	std::string m_str;
	void clean()
	{
		for (int m = 0; m != m_str.size(); ++m)
			if (m_str[m] == '\n')
				m_str[m] = '\0';
	}
  public:
	c_color text, back;
	event onTextChange;
	c_text()
	{
		ControlManager.add(this);
		text.set_surface(color::ground::fore);
		back.set_surface(color::ground::back);
		text = color::white;
		back = color::black;
	}
	c_text(const std::string txt)
	{
		ControlManager.add(this);
		text.set_surface(color::ground::fore);
		back.set_surface(color::ground::back);
		text = color::white;
		back = color::black;
		m_str = txt;
		clean();
		m_render.set(m_str.size(), 1);
	}
	c_text & operator=(const std::string txt)
	{
		m_str = txt;
		clean();
		m_render.set(m_str.size(), 1);
		onTextChange.happen();
		return *this;
	}
	baseImage & draw()
	{
		cgl::point tmp;
		tmp.text = text;
		tmp.back = back;
		for (int n = 0; n != m_str.size(); ++n)
		{
			tmp = m_str[n];
			m_render.draw(n, 0, tmp);
		}
		return m_render;
	}
	const std::string & txt()
	{
		return m_str;
	}
	size_t size()
	{
		return m_str.size();
	}
};


class cgl::button:public baseControl
{
  protected:
	std::string m_str;
	char m_border;
	int m_wid, m_hig;
  public:
	event onButtonClick;
	event onTextChange;
	event onBorderChange;
	c_color text, back, borderTxt;
	button():m_border(' ')
	{
		ControlManager.add(this);
		text.set_surface(color::ground::fore);
		back.set_surface(color::ground::back);
		borderTxt.set_surface(color::ground::fore);
		text = color::black;
		back = color::white;
		borderTxt = text;
		m_str = "Button";
		m_wid = m_str.size() + 4;
		m_hig = 3;
		m_render.alpha(false);
		m_render.set(m_wid, m_hig);
		m_render.reset();
	}
	button(int w, int h)
	{
		ControlManager.add(this);
		text.set_surface(color::ground::fore);
		back.set_surface(color::ground::back);
		borderTxt.set_surface(color::ground::fore);
		text = color::black;
		back = color::white;
		borderTxt = text;
		m_str = "Button";
		m_wid = w;
		m_hig = h;
		m_render.alpha(false);
		m_render.set(m_wid, m_hig);
		m_render.reset();
	}
	~button()
	{
		ControlManager.remove(this);
	}
	void txt(const std::string & str)
	{
		m_str = str;
		onTextChange.happen();
	}
	const std::string & txt()
	{
		return m_str;
	}
	void border(char ch)
	{
		m_border = ch;
		onBorderChange.happen();
	}
	char border()
	{
		return m_border;
	}
	void click()
	{
		onButtonClick.happen();
	}
	int width() const
	{
		return m_render.width();
	}
	int high() const
	{
		return m_render.high();
	}
	void resize(int w, int h)
	{
		m_wid = w;
		m_hig = h;
		m_render.set(m_wid, m_hig);
	}
	void resize_from_txt()
	{
		m_wid = m_str.size() + 4;
		m_hig = 3;
		m_render.set(m_wid, m_hig);
	}
	baseImage & draw()
	{
		m_render.back(back);
		m_render.reset();
		point nib;
		nib.text = text;
		nib.back = back;
		int txtx = m_wid / 2 - m_str.size() / 2;
		int txty = (m_hig - 1) / 2;
		for (int n = 0; n != m_str.size(); ++n)
		{
			nib = m_str[n];
			m_render.draw(txtx + n, txty, nib);
		}
		nib = m_border;
		nib.text = borderTxt;
		for (int i = 0; i < m_render.width(); ++i)
		{
			m_render.draw(i, 0, nib);
			m_render.draw(i, m_render.high() - 1, nib);
		}
		for (int i = 0; i < m_render.high(); ++i)
		{
			m_render.draw(0, i, nib);
			m_render.draw(m_render.width() - 1, i, nib);
		}
		return m_render;
	}
};


#endif