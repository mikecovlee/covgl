#ifndef CGL_GRAPHICS_KERNEL_H


#define CGL_GRAPHICS_KERNEL_H


// Covariant Graphics Kernel 2


#include "CovDevKit.h"
#include "CovThread.h"


#ifdef CGL_TRACK


#define CGL_DEBUG


#endif


namespace cgl
{
#ifdef CGL_DEBUG
	static const bool debug = true;
#else
	static const bool debug = false;
#endif
}


#ifdef CGL_DEBUG
#include <fstream>
#endif
#include <cstdlib>
#include <cstdio>
#include <string>
#include <vector>
#include <ctime>
#include <list>
#include <map>


namespace cgl
{
	namespace ioctrl
	{
		using namespace cov::ioctrl;
	}
	using cov::print;
	using cov::random;
	  template < typename > class queue;
	class baseClass;
	unsigned long events_ident_code = 0;
	class event;
	class baseControl;
	class univCtrlMan;
}


template < typename T > class cgl::queue
{
  protected:
	std::vector < T > mList;
	std::map < T, int >mQueue;
  public:
	void add(const T & key)
	{
		mQueue[key] = mQueue.size();
	}
	void focus(const T & key)
	{
		int tmp = 0;
	  for (auto & it:mQueue)
		{
			if (it.first == key)
			{
				if (it.second > 1)
					tmp = it.second;
				else
					return;
			}
		}
	  for (auto & it:mQueue)
		{
			if (it.first == key)
				it.second = 1;
			else if (it.second < tmp)
				++it.second;
		}
	}
	void remove(const T & key)
	{
		focus(key);
		mQueue.erase(key);
	  for (auto & it:mQueue)
			--it.second;
	}
	std::vector < T > &get_queue()
	{
		mList.resize(mQueue.size());
	  for (auto it:mQueue)
			mList[it.second - 1] = it.first;
		return mList;
	}
};


class cgl::baseClass
{
  public:
	baseClass() = default;
	baseClass(const baseClass &) = default;
	virtual ~ baseClass()
	{
	};
};


class cgl::event
{
	unsigned long m_ident_code = ++events_ident_code;
	bool m_happened = false;
  public:
	event() = default;
	event(const event & ev):m_ident_code(ev.m_ident_code)
	{
	}
	void happen()
	{
		m_happened = true;
	}
	void finish()
	{
		m_happened = false;
	}
	bool demand()
	{
		bool ret = m_happened;
		m_happened = false;
		return ret;
	}
	const unsigned long operator~()
	{
		return m_ident_code;
	}
	bool operator==(const event & ev)
	{
		return m_ident_code == ev.m_ident_code;
	}
	bool operator!=(const event & ev)
	{
		return m_ident_code != ev.m_ident_code;
	}
	event & operator=(const event & ev)
	{
		m_ident_code = ev.m_ident_code;
		m_happened = ev.m_happened;
		return *this;
	}
};


class cgl::univCtrlMan:public baseClass
{
  protected:
	queue < baseControl * >m_que;
	baseControl *m_selected;
  public:
	void add(baseControl * it)
	{
		m_que.add(it);
	}
	void remove(baseControl * it)
	{
		m_que.remove(it);
	}
	void focus(baseControl * it)
	{
		m_que.focus(it);
		m_selected = it;
	}
	void focus(baseControl & it)
	{
		m_que.focus(&it);
		m_selected = &it;
	}
	inline void select(baseControl *);
	inline void select(baseControl &);
	bool selected(baseControl * it)
	{
		return it == m_selected;
	}
	bool selected(baseControl & it)
	{
		return &it == m_selected;
	}
	baseControl *selected()
	{
		return m_selected;
	}
	template < typename Scr_Type, typename Out_Type > inline void display(Scr_Type &, Out_Type &);
};


namespace cgl
{
	static univCtrlMan ControlManager;
}


#include "Clock.h"


namespace cgl
{
	namespace track
	{
#ifdef CGL_TRACK
		enum rank
		{
			normal = 0x76a,
			caution = 0x76b,
			error = 0x76c
		};
		class message
		{
			std::string m_msg;
			int m_level;
		  public:
			message():m_level(rank::normal)
			{
			}
			message(const std::string & msg):m_level(rank::normal)
			{
				m_msg = msg;
			}
			message(const std::string & msg, int lev):m_level(lev)
			{
				m_msg = msg;
			}
			bool operator==(const message & obj)
			{
				return m_msg == obj.m_msg && m_level == obj.m_level;
			}
			bool operator!=(const message & obj)
			{
				return m_msg == obj.m_msg && m_level == obj.m_level;
			}
			operator          const char *()
			{
				return m_msg.c_str();
			}
			message & operator=(const std::string & msg)
			{
				m_msg = msg;
				return *this;
			}
			int level()
			{
				return m_level;
			}
			void level(int lev)
			{
				m_level = lev;
			}
		};
		class msgsocket
		{
			std::list < message > msgs;
			int receive_level;
			baseClass *receive_target;
		  public:
			msgsocket() = delete;
			msgsocket(const msgsocket &) = delete;
			msgsocket(int lev):receive_level(lev), receive_target(0)
			{
			}
			void target(baseClass * obj)
			{
				receive_target = obj;
			}
			void send(message & msg, baseClass * obj = 0)
			{
				if (receive_target)
				{
					if (receive_target == obj)
						msgs.push_back(msg);
				}
				else if (msg.level() == receive_level)
					msgs.push_back(msg);
			}
			bool receive(message & msg)
			{
				bool have = false;
			  for (auto & it:msgs)
				{
					if (it == msg)
					{
						have = true;
						break;
					}
				}
				if (have)
					msgs.remove(msg);
				return have;
			}
			bool log_to_file(const char *dir)
			{
				if (msgs.size() == 0)
					return false;
				std::ofstream out(dir);
				if (!out)
					return false;
			  for (auto & it:msgs)
				{
					switch (it.level())
					{
					case rank::normal:
						out << "Normal Message:" << it << std::endl;
						break;
					case rank::caution:
						out << "Caution Message:" << it << std::endl;
						break;
					case rank::error:
						out << "Error Message:" << it << std::endl;
						break;
					}
				}
				return true;
			}
		};
		static msgsocket normal_msg(rank::normal);
		static msgsocket caution_msg(rank::caution);
		static msgsocket error_msg(rank::error);
		inline void execute(msgsocket &, const unsigned int, const char *, message &, baseClass *);
#endif
		inline void stop(const std::string &);
	}
}


#ifdef CGL_TRACK


void cgl::track::execute(msgsocket & msgs, const unsigned int line, const char *func,
						 message & msg, baseClass * obj = 0)
{
	char tmp[16];
	auto int2str =[&](int i){
		sprintf(tmp, "%d", i);
		return std::string(tmp);
	};
	std::string str;
	str =
		"TIME:" + int2str(cgl::clock.rt_sec()) + " LINE:" + int2str(line) + " FUNC:" + func +
		" MSG:";
	str.append(msg);
	msg = str;
	msgs.send(msg, obj);
}


#define track_to_normal(msg) cgl::track::execute(cgl::track::normal_msg,__LINE__,__func__,msg);
#define track_to_caution(msg) cgl::track::execute(cgl::track::caution_msg,__LINE__,__func__,msg);
#define track_to_error(msg) cgl::track::execute(cgl::track::error_msg,__LINE__,__func__,msg);
#define _track_to_normal(msg,obj) cgl::track::execute(cgl::track::normal_msg,__LINE__,__func__,msg);
#define _track_to_caution(msg,obj) cgl::track::execute(cgl::track::caution_msg,__LINE__,__func__,msg,obj);
#define _track_to_error(msg,obj) cgl::track::execute(cgl::track::error_msg,__LINE__,__func__,msg,obj);


#endif


void cgl::track::stop(const std::string & msg)
{
	print("Covariant GL has already terminated this program.\n");
	print(msg.c_str(), ioctrl::endl);
	exit(0);
}


#endif