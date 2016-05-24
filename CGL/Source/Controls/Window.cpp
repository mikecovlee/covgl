#ifndef CGL_WINDOW_MAIN_CPP


#define CGL_WINDOW_MAIN_CPP


#include "Window.h"


cgl::window::window(int w, int h)
{
	object = 0;
	m_title = "CGL Window";
	m_border = '+';
	m_width = w;
	m_high = h;
	m_texture.set(m_width, m_high);
	int wid = m_width - 2;
	int hig = m_high - 4;
	m_render.set(wid, hig);
	create();
}


cgl::window::window(baseView & view)
{
	object = &view;
	m_title = "CGL Window";
	m_border = '+';
	m_width = view.width();
	m_high = view.high();
	m_texture.set(m_width, m_high);
	int wid = m_width - 2;
	int hig = m_high - 4;
	m_render.set(wid, hig);
	create();
}


cgl::baseImage & cgl::window::draw()
{
	if (df_available)
		draw_func(*this);
	bool mSelected = ControlManager.selected(this);
	std::string oldt = m_title;
	m_texture.reset();
	if (AutoWindowAdaptate && object)
	{
		m_width = object->width();
		m_high = object->high();
		int wid = m_width - 2;
		int hig = m_high - 4;
		m_render.set(wid, hig);
		m_texture.set(m_width, m_high);
	}
	cgl::point tmp;
	tmp = m_border;
	if (mSelected && debug)
		m_title += " - Selected";
	tmp.text = m_border_color;
	tmp.back = m_texture.back();
	for (int i = 0; i < m_texture.width(); ++i)
	{
		m_texture.draw(i, 0, tmp);
		m_texture.draw(i, 2, tmp);
		m_texture.draw(i, m_texture.high() - 1, tmp);
	}
	for (int i = 0; i < m_texture.high(); ++i)
	{
		m_texture.draw(0, i, tmp);
		m_texture.draw(m_texture.width() - 1, i, tmp);
	}
	for (int n = 0; n != m_title.size(); ++n)
	{
		tmp = m_title[n];
		if (n >= width())
			break;
		m_texture.draw(n + 1, 1, tmp);
	}
	m_texture.draw(1, 3, m_render);
	m_title = oldt;
	return m_texture;
}


#endif