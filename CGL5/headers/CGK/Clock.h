#ifndef CGL_CGK_CLOCK_H


#define CGL_CGK_CLOCK_H


#include <thread>
#include <chrono>


namespace cgl
{
	enum time
	{
		ms = 0,
		sec = 1,
		min = 2,
	};
	class c_time;
}


class cgl::c_time
{
	std::chrono::time_point < std::chrono::high_resolution_clock > m_timer;
  public:
	c_time():m_timer(std::chrono::high_resolution_clock::now())
	{
	}
	void reset()
	{
		m_timer = std::chrono::high_resolution_clock::now();
	}
	unsigned long rt_ms()
	{
		return std::chrono::duration_cast < std::chrono::microseconds >
			(std::chrono::high_resolution_clock::now() - m_timer).count();
	}
	unsigned int rt_sec()
	{
		return std::chrono::duration_cast < std::chrono::seconds >
			(std::chrono::high_resolution_clock::now() - m_timer).count();
	}
	unsigned int rt_min()
	{
		return std::chrono::duration_cast < std::chrono::minutes >
			(std::chrono::high_resolution_clock::now() - m_timer).count();
	}
	void delay(const unsigned int unit, const unsigned long time)
	{
		switch (unit)
		{
		case 0:
			{
				unsigned long start, end;
				start = rt_ms();
				do
				{
					end = rt_ms();
					std::this_thread::yield();
				}
				while ((end - start) < time);
				break;
			}
		case 1:
			{
				unsigned int start, end;
				start = rt_sec();
				do
				{
					end = rt_sec();
					std::this_thread::yield();
				}
				while ((end - start) < time);
				break;
			}
		case 2:
			{
				unsigned int start, end;
				start = rt_min();
				do
				{
					end = rt_min();
					std::this_thread::yield();
				}
				while ((end - start) < time);
				break;
			}
		}
	}
	template < typename Func, typename...Args >
		void delay(const unsigned int unit, const unsigned long time, Func func, Args ... args)
	{
		switch (unit)
		{
		case 0:
			{
				unsigned long start, end;
				start = rt_ms();
				do
				{
					end = rt_ms();
					std::this_thread::yield();
					func(args...);
				}
				while ((end - start) < time);
				break;
			}
		case 1:
			{
				unsigned int start, end;
				start = rt_sec();
				do
				{
					end = rt_sec();
					std::this_thread::yield();
					func(args...);
				}
				while ((end - start) < time);
				break;
			}
		case 2:
			{
				unsigned int start, end;
				start = rt_min();
				do
				{
					end = rt_min();
					std::this_thread::yield();
					func(args...);
				}
				while ((end - start) < time);
				break;
			}
		}
	}
};


namespace cgl
{
	static c_time clock;
}


#endif