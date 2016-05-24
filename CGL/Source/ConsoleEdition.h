#ifndef GUARD_CGL5_CE_H


#define GUARD_CGL5_CE_H


namespace cgl
{
	typedef baseImage surface;
	typedef baseView screen;
	  template < typename > class console;
}


template < typename Obj > class cgl::console
{
  protected:
	bool m_input_mod = false;
	Obj *object;
	std::list < point > __buff;
	std::list < point > input_buff;
	std::list < std::list < point >> __sbuf;
	char *str_buff = 0;
	point __cursor;
	void recover()
	{
		if (!m_input_mod && !input_buff.empty())
		{
		  for (auto & it:input_buff)
				__buff.push_back(it);
			input_buff.clear();
		}
		while (__buff.size() > object->high() * object->width())
			__buff.pop_front();
	}
	void output_scr()
	{
		out << *object;
	}
	void print_to_scr()
	{
		object->reset();
		__sbuf.clear();
		std::list < point > swap;
		for (auto it = __buff.begin(); it != __buff.end(); ++it)
		{
			if (swap.size() == object->width())
			{
				__sbuf.push_back(swap);
				swap.clear();
			}
			if (it->data() == '\n')
			{
				__sbuf.push_back(swap);
				swap.clear();
				auto tmp = it;
				if (++tmp == __buff.end())
				{
					if (input_buff.empty())
						__sbuf.push_back(swap);
					break;
				}
			}
			else
				swap.push_back(*it);
		}
		for (auto it = input_buff.begin(); it != input_buff.end(); ++it)
		{
			if (swap.size() == object->width())
			{
				__sbuf.push_back(swap);
				swap.clear();
			}
			if (it->data() == '\n')
			{
				__sbuf.push_back(swap);
				swap.clear();
				auto tmp = it;
				if (++tmp == input_buff.end())
				{
					__sbuf.push_back(swap);
					break;
				}
			}
			else
				swap.push_back(*it);
		}
		if (!swap.empty())
			__sbuf.push_back(swap);
		while (__sbuf.size() > object->high())
			__sbuf.pop_front();
		int px, py;
		py = __sbuf.size() - 1;
		for (auto it = __sbuf.rbegin(); it != __sbuf.rend() && py >= 0; ++it)
		{
			px = it->size() - 1;
			for (auto tp = it->rbegin(); tp != it->rend(); ++tp)
			{
				object->draw(px, py, tp->data());
				--px;
			}
			--py;
		}
		if (!__buff.empty())
		{
			if (__sbuf.back().size() == object->width() && __sbuf.size() < object->high() - 1)
				object->draw(0, __sbuf.size(), '_');
			else
				object->draw(__sbuf.back().size(), __sbuf.size() - 1, '_');
		}
		else
			object->draw(0, 0, '_');
		output_scr();
	}
  public:
	console() = delete;
  console(Obj & o):object(&o)
	{
	}
	~console()
	{
		delete[]str_buff;
	}
	void SetColor(char color, char ground = color::ground::fore)
	{
		switch (ground)
		{
		case color::ground::fore:
			__cursor.text = color;
			break;
		case color::ground::back:
			__cursor.back = color;
			break;
		}
	}
	void SetAttri(char attri)
	{
		__cursor.attribute(attri);
	}
	void ClrScr()
	{
		__buff.clear();
		input_buff.clear();
		object->reset();
		output_scr();
	}
	void input_mod(bool mod)
	{
		m_input_mod = mod;
	}
	bool input_mod()
	{
		return m_input_mod;
	}
	const char *input()
	{
		delete[]str_buff;
		str_buff = new char[input_buff.size() + 1];
		char *tmp = str_buff;
	  for (auto & it:input_buff)
		{
			if (it.data() != '\n')
			{
				*tmp = it.data();
				++tmp;
			}
		}
		*tmp = '\0';
		return str_buff;
	}
	void print(cov::generic_type gt)
	{
		const char *str = gt.val();
		for (int i = 0; i < cov::strlen(str); ++i)
		{
			__cursor = str[i];
			if (str[i] == (char)keyboard::back_space)
			{
				if ((!input_buff.empty()) && m_input_mod)
					input_buff.pop_back();
			}
			else
			{
				if (m_input_mod)
					input_buff.push_back(__cursor);
				else
					__buff.push_back(__cursor);
				if (str[i] == '\n')
					m_input_mod = false;
			}
		}
	}
	void print(void (*func) ())
	{
		if (func == ioctrl::endl)
		{
			m_input_mod = false;
			print('\n');
		}
		if (func == ioctrl::flush)
			print_to_scr();
	}
	void output()
	{
		print_to_scr();
		output_scr();
	}
	template < typename T, typename...Argt > void output(const T & obj, const Argt & ... args)
	{
		recover();
		print(obj);
		output(args...);
	}
	console & operator<<(cov::generic_type obj)
	{
		recover();
		print(obj);
		return *this;
	}
	console & operator<<(void (*func) ())
	{
		recover();
		print(func);
		return *this;
	}
};


namespace cgl
{
	template <> void console < window >::output_scr()
	{
		ControlManager.display(scr, out);
	}
}


#endif