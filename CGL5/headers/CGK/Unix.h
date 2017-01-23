#ifndef CGL_CPGE_UNIX_H


#define CGL_CPGE_UNIX_H


#include <unistd.h>
#include <termios.h>
#include <sys/ioctl.h>
#include <sys/select.h>


inline int cgl::get_terminal_width()
{
	struct winsize size;
	ioctl(STDIN_FILENO, TIOCGWINSZ, &size);
	return size.ws_col;
}


inline int cgl::get_terminal_high()
{
	struct winsize size;
	ioctl(STDIN_FILENO, TIOCGWINSZ, &size);
	return size.ws_row - 1;
}


inline void cgl::reset_character_attri()
{
	print("\e[37;40;0m");
}


inline void cgl::set_character_attri(char *attri)
{
	if (attri[0] == '0')
		print("\e[", attri[1], attri[2], ';', attri[3], attri[4], "m");
	else
		print("\e[", attri[1], attri[2], ';', attri[3], attri[4], ';', attri[0], 'm');
}


bool cgl::basic::KbHit()
{
	bool ret;
	fd_set fds;
	if (m_echo)
	{
		struct termios oldt, newt;
		tcgetattr(0, &oldt);
		newt = oldt;
		newt.c_lflag &= ~ICANON;
		newt.c_lflag &= ~ECHO;
		tcsetattr(0, TCSANOW, &newt);
		ret = getchar();
		tcsetattr(0, TCSANOW, &oldt);
	}
	struct timeval tv;
	tv.tv_sec = 0;
	tv.tv_usec = 0;
	FD_ZERO(&fds);
	FD_SET(0, &fds);
	select(1, &fds, 0, 0, &tv);
	ret = FD_ISSET(0, &fds);
	return ret;
}


int cgl::basic::GetKbHit()
{
	int ret;
	if (m_echo)
	{
		struct termios oldt, newt;
		tcgetattr(0, &oldt);
		newt = oldt;
		newt.c_lflag &= ~ICANON;
		newt.c_lflag &= ~ECHO;
		tcsetattr(0, TCSANOW, &newt);
		ret = getchar();
		tcsetattr(0, TCSANOW, &oldt);
	}
	else
		ret = getchar();
	return ret;
}


int cgl::basic::GetKbHitPer(unsigned long wait = 0)
{
	unsigned long start, stop;
	start = stop = clock.rt_ms();
	do
	{
		stop = clock.rt_ms();
		if (KbHit())
			return GetKbHit();
	}
	while ((stop - start) < wait);
	return -1;
}


void cgl::basic::Echo(bool in)
{
	flush();
	struct termios oldt, newt;
	tcgetattr(0, &oldt);
	newt = oldt;
	if (in)
	{
		print("\33[?25h", ioctrl::flush);
		newt.c_lflag |= ECHO;
		newt.c_lflag |= ICANON;
		m_echo = true;
	}
	else
	{
		print("\33[?25l", ioctrl::flush);
		newt.c_lflag &= ~ECHO;
		newt.c_lflag &= ~ICANON;
		m_echo = false;
	}
	tcsetattr(0, TCSANOW, &newt);
}


void cgl::basic::rst_cursor()
{
	print("\x1B[0;0f", ioctrl::flush);
}


void cgl::basic::ClrScr()
{
	print("\x1B[2J\x1B[0;0f", ioctrl::flush);
}


#endif