#ifndef CGL_CPGE_BASIC_H


#define CGL_CPGE_BASIC_H


#ifdef CGL_DEBUG
#include <string>
#endif


namespace cgl
{
	namespace priv_realize
	{
		void assign(char *array)
		{
		}
		template < typename ... Argt >
			inline void assign(char *array, const char ch, const Argt & ... args)
		{
			*array = ch;
			if (ch == '\0')
				return;
			++array;
			assign(array, args ...);
		}

#ifdef CGL_DEBUG
		std::string int2str(int);
#endif
	}
	inline double div(double a, double b)
	{
		if (b == 0)
			return a;
		return a / b;
	}
}


#ifdef CGL_DEBUG
std::string cgl::priv_realize::int2str(int n)
{
	char tmp[16];
	sprintf(tmp, "%d", n);
	return std::string(tmp);
}
#endif


class cgl::basic
{
	class frame
	{
	  protected:
		long t_start = 0;
		long t_end = 0;
		int m_frame;
	  public:
		frame():m_frame(28)
		{
		} frame(int frp):m_frame(frp)
		{
		} void init()
		{
			t_start = clock.rt_ms();
		}
		void ctrl(bool, unsigned int);
		void ctrl(unsigned int);
	};
#ifdef CGL_DEBUG
	unsigned long tm_begin = clock.rt_sec();
	int FPS = 0;
#endif
	bool m_echo = true;
	frame fpsctrl;
	void rst_cursor();
	void flush()
	{
		fflush(stdout);
	}
  public:
	void ClrScr();
	void create()
	{;
		flush();
		fflush(stdin);
		Echo(false);
		ClrScr();
		rst_cursor();
	}
	void destroy()
	{
		flush();
		fflush(stdin);
		Echo(true);
		ClrScr();
		rst_cursor();;
	}
	unsigned int limit_fps = 60;
	bool Afrct = true;
	bool frame_limit = true;
	bool auto_adapt_scr = true;
	basic() = default;
	~basic()
	{
		destroy();
	}
	bool KbHit();
	int GetKbHit();
	int GetKbHitPer(unsigned long);
	void Echo(bool);
	bool Echo() const
	{
		return m_echo;
	}
	void fresh()
	{
#ifdef CGL_DEBUG
		tm_begin = clock.rt_sec();
		FPS = 0;
#endif
	}
	inline basic & operator<<(baseView &);
};


void cgl::basic::frame::ctrl(bool frame_limit = true, unsigned int lmf = 60)
{
	t_end = clock.rt_ms();
	int time_used = t_end - t_start;
	int safe_fps = div(1000000, time_used);
	safe_fps *= 0.65;
	if (frame_limit)
	{
		if (safe_fps > lmf)
			safe_fps = lmf;
	}
	if (time_used < div(1000000, safe_fps))
	{
		clock.delay(time::ms, div(1000000, safe_fps) - time_used);
	}
}


void cgl::basic::frame::ctrl(unsigned int frp)
{
	t_end = clock.rt_ms();
	if ((t_end - t_start) < div(1000000, frp))
	{
		clock.delay(time::ms, div(1000000, frp) - (t_end - t_start));
	}
}


inline cgl::basic & cgl::basic::operator<<(baseView & in)
{
	using namespace priv_realize;
	if (auto_adapt_scr)
		in.adaptate();
	char attri[6];
	fpsctrl.init();
	flush();
	rst_cursor();
	c_image < point > &img = in.image();
  for (auto & row:img.array())
	{
	  for (auto & nib:row)
		{
			if (nib < 32 || nib > 126)
				nib = '\?';
			priv_realize::assign(attri, nib.attribute(), nib.text.data()[0],
								 nib.text.data()[1], nib.back.data()[0], nib.back.data()[1], '\0');
			set_character_attri(attri);
			print(nib.data());
			reset_character_attri();
		}
		reset_character_attri();
		print(ioctrl::endl);
	}

#ifdef CGL_DEBUG
	++FPS;
	if (clock.rt_sec() - tm_begin >= 1)
	{
		std::string information;
		information = "FPS:" + int2str(FPS) +
			" VRR:(" + int2str(get_terminal_width()) + "*" + int2str(get_terminal_high()) +
			") RT:" + int2str(clock.rt_sec()) + "s";
		std::string space(in.width() - information.size(), ' ');
		print(information.c_str(), space.c_str(), ioctrl::flush);
		FPS = 0;
		tm_begin = clock.rt_sec();
	}
#endif
	if (Afrct)
		fpsctrl.ctrl(frame_limit, limit_fps);
	else
		fpsctrl.ctrl(limit_fps);
	return *this;
}


#endif