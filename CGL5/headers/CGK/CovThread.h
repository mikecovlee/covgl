#ifndef COV_THREAD_H


#define COV_THREAD_H


#include <atomic>
#include <thread>
#include <mutex>


namespace cov
{
	class thread;
	using std::atomic;
}


class cov::thread
{
  protected:
	std::thread m_thread;
	std::mutex m_locker;
	std::atomic < int >m_state;
	std::atomic < int >m_flag;
	std::atomic < bool > m_leisure;
	std::atomic < bool > m_complete;
	std::atomic < const char *>m_name;
	template < typename T, typename Atomic_T = std::atomic < T >>
		void store_value(Atomic_T & obj, const T & value)
	{
		while (!obj.is_lock_free())
			std::this_thread::yield();
		obj.store(value);
	}
  public:
	enum states
	{
		onBusy = 0x010a, onRunning = 0x010b, onPause = 0x010c, onSuspend = 0x010d, onHalt = 0x010e
	};
	thread(const char *name, void (*func) ()):m_thread(func), m_state(states::onHalt), m_flag(0),
		m_leisure(false), m_complete(false), m_name(name)
	{
	}
	~thread()
	{
	}
	const char *name()
	{
		return m_name.load();
	}
	void locker(bool key)
	{
		if (key)
			m_locker.lock();
		else
			m_locker.unlock();
	}
	void join()
	{
		store_value(m_state, states::onBusy);
		m_thread.join();
	}
	void detach()
	{
		store_value(m_state, states::onBusy);
		m_thread.detach();
	}
	void leisure(bool set)
	{
		store_value(m_leisure, set);
	}
	bool leisure()
	{
		return m_leisure.load();
	}
	void resume()
	{
		store_value(m_state, states::onRunning);
	}
	void pause()
	{
		store_value(m_state, states::onPause);
	}
	void suspend()
	{
		store_value(m_state, states::onSuspend);
	}
	void complete(int flag)
	{
		store_value(m_state, states::onHalt);
		store_value(m_complete, true);
		store_value(m_flag, flag);
	}
	bool complete()
	{
		return m_complete.load();
	}
	int flag()
	{
		return m_flag.load();
	}
	int state()
	{
		return m_state.load();
	}
};


#define ThrName(name) CovThFunc_ ## name
#define ObjName(name) CovThrObj_ ## name


#define ThreadSuccess 0
#define ThreadFailure 1
#define ThreadInterrupt 2


#define Thread(name) ObjName(name)
#define CreateThread(name) void ThrName(name)();static cov::thread ObjName(name)("Thread_"#name,ThrName(name));void ThrName(name)()
#define ThreadIdle std::this_thread::yield();
#define EndThread(name,flag) ObjName(name).complete(flag);
#define InterruptThread(name) EndThread(name,ThreadInterrupt)
#define WaitForLeisure(name) while(!Thread(name).leisure()){std::this_thread::yield();}
#define WaitForExit(name) while(!Thread(name).complete()){std::this_thread::yield();}


#define Loop(name) while(Thread(name).state()!=cov::thread::states::onSuspend){Thread(name).locker(true);
#define EndLoop(name) Thread(name).locker(false);}
#define SitLoop(name) while(!Thread(name).leisure()){Thread(name).leisure(true);}while(Thread(name).state()!=cov::thread::states::onSuspend){Thread(name).locker(true);switch(Thread(name).state()){
#define EndSitLoop(name) }}Thread(name).locker(false);
#define BreakOffLoop(name) Thread(name).suspend();
#define DefaultSit default:
#define OnRunningSit case cov::thread::states::onRunning:
#define OnPauseSit case cov::thread::states::onPause:
#define EndSit break;


#endif