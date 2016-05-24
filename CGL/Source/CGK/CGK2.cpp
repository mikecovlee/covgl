#ifndef CGL_CGK2_CPP


#define CGL_CGK2_CPP


#include "CGK2.h"
#include "Compiler.h"


void cgl::univCtrlMan::select(baseControl * it)
{
	m_selected = it;
	m_selected->onCtrlSelected.happen();
}


void cgl::univCtrlMan::select(baseControl & it)
{
	m_selected = &it;
	m_selected->onCtrlSelected.happen();
}


template < typename T > void cgl::c_image < T >::resize(int col, int row)
{
	data.resize(row);
  for (auto & it:data)
		it.resize(col);
}


template < typename T > cgl::c_image < T >::c_image(const c_image & in)
#ifdef CGL_TRACK
:out_of_range("out of range", track::rank::error)
#endif
{
	wid = in.width();
	hig = in.high();
	data = in.data;
}


template < typename T > void cgl::c_image < T >::set(int w, int h)
{
	wid = w;
	hig = h;
	resize(w, h);
}


template < typename T > cgl::c_image < T > &cgl::c_image < T >::operator=(const c_image & in)
{
	if (&in != this)
	{
		wid = in.width();
		hig = in.high();
		data = in.data;
	}
}


template < typename T > T & cgl::c_image < T >::read(int x, int y)
{
	if (x < 0 || y < 0 || x >= width() || y >= high())
	{
#ifdef CGL_TRACK
		_track_to_error(out_of_range, this);
#endif
		throw 0;
	}
	else
	{
		return data[y][x];
	}
}


template < typename T > void cgl::c_image < T >::write(int x, int y, T in)
{
	if (x < 0 || y < 0 || x >= width() || y >= high())
	{
#ifdef CGL_TRACK
		_track_to_error(out_of_range, this);
#endif
		throw 0;
	}
	else
	{
		data[y][x] = in;
	}
}


int cgl::position::operator++()
{
	++m_position;
	onPositionMove.happen();
	return m_position;
}


int cgl::position::operator--()
{
	--m_position;
	onPositionMove.happen();
	return m_position;
}


cgl::c_image < cgl::point > &cgl::baseImage::remix()
{
	if (!m_use)
	{
#ifdef CGL_TRACK
		_track_to_error(use_without_init, this);
#endif
		throw 0;
	}
	if (m_alpha)
		return m_image;
	tmp_image.set(m_image.width(), m_image.high());
	point tmp;
	tmp.alpha(false);
	tmp = ' ';
	tmp.back = m_back;
	for (int i = 0; i != tmp_image.high(); ++i)
	{
		for (int k = 0; k != tmp_image.width(); ++k)
		{
			if (m_image.read(k, i).alpha())
				tmp_image.write(k, i, tmp);
			else
				tmp_image.write(k, i, m_image.read(k, i));
		}
	}
	return tmp_image;
}


void cgl::baseImage::reset()
{
	if (!m_use)
		return;
	m_blank = true;
	point tmp;
	tmp.alpha(true);
	tmp = ' ';
  for (auto & row:m_image.array())
	  for (auto & nib:row)
			nib = tmp;
}


void cgl::baseImage::set(int w, int h)
{
	m_use = true;
	if (m_blank)
	{
		m_image.set(w, h);
		reset();
	}
	else
	{
		bool blank_before = m_blank;
		c_image < point > tmp = m_image;
		m_image.set(w, h);
		reset();
		if (!blank_before)
		{
			int i, k, m, n;
			point pen;
			pen = ' ';
			pen.back = m_back;
			for (i = 0, m = 0; m < m_image.high() && i < tmp.high(); ++i, ++m)
			{
				for (k = 0, n = 0; n < m_image.width() && k < tmp.width(); ++k, ++n)
				{
					if (m_alpha)
					{
						if (!tmp.read(k, i).alpha())
							draw(n, m, tmp.read(k, i));
					}
					else
					{
						if (tmp.read(k, i).alpha())
							draw(n, m, pen);
						else
							draw(n, m, tmp.read(k, i));
					}
				}
			}
		}
	}
}


cgl::point & cgl::baseImage::read(int x, int y)
{
	return m_image.read(x, y);
}


bool cgl::baseImage::draw(int x, int y, char ch, char attri = attribute::none)
{
	if (!m_use || x < 0 || y < 0 || x >= m_image.width() || y >= m_image.high())
		return false;
	m_blank = false;
	point tmp;
	tmp = ch;
	tmp.attribute(attri);
	if (ch != ' ')
		tmp.back = m_back;
	m_image.write(x, y, tmp);
	return true;
}


bool cgl::baseImage::draw(int x, int y, point & tmp)
{
	if (!m_use || x < 0 || y < 0 || x >= m_image.width() || y >= m_image.high())
		return false;
	m_blank = false;
	m_image.write(x, y, tmp);
	return true;
}


bool cgl::baseImage::draw(int x, int y, baseImage & tmp)
{
	if (!tmp.use() || x + tmp.width() < 0 || y + tmp.high() < 0
		|| x >= m_image.width() || y >= m_image.high())
		return false;
	m_blank = false;
	int i, k, m, n;
	point pen;
	pen = ' ';
	pen.back = tmp.back();
	for (i = 0, m = y; m < m_image.high() && i < tmp.high(); ++i, ++m)
	{
		for (k = 0, n = x; n < m_image.width() && k < tmp.width(); ++k, ++n)
		{
			if (tmp.alpha())
			{
				if (!tmp.read(k, i).alpha())
					draw(n, m, tmp.read(k, i));
			}
			else
			{
				if (tmp.read(k, i).alpha())
					draw(n, m, pen);
				else
					draw(n, m, tmp.read(k, i));
			}
		}
	}
	return true;
}


cgl::baseControl::baseControl()
{
	ControlManager.add(this);
}


cgl::baseControl::baseControl(int t_wid, int t_hig)
{
	ControlManager.add(this);
	m_render.set(t_wid, t_hig);
}


template < typename Scr_Type, typename Out_Type >
	void cgl::univCtrlMan::display(Scr_Type & scr, Out_Type & out)
{
	std::vector < baseControl * >&list = m_que.get_queue();
	for (auto it = list.rbegin(); it != list.rend(); ++it)
		if (*it)
			if ((*it)->visible())
				scr << *(*it);
	out << scr;
}


#endif