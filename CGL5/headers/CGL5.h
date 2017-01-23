#ifndef GUARD_CGL5_H


#define GUARD_CGL5_H


// Covariant Graphics Library V
// Compile by Mike Covariant Lee
// 李登淳编写


#include "./CGK/CGK2.cpp"
#include "./Controls/Controls.h"
#include "./Controls/Window.cpp"
#include "./Keyboard_Layout.h"


namespace cgl
{
	const char *cgl_version = "5.16.1.1 Beta";
	static baseView scr;
	static basic out;
}


#include "./ConsoleEdition.h"


namespace cgl
{
	static console < screen > cons(scr);
	void boot();
	void exit();
	  template < typename Func, typename ... Argt > int launch(const Func &, const Argt & ...);
	  template < typename Ret, typename Func, typename ... Argt >
		int launch(Ret &, const Func &, const Argt & ...);
}


void cgl::boot()
{
	out.create();
	out.ClrScr();
	scr.adaptate();
}


void cgl::exit()
{
	out.destroy();
}


template < typename Func, typename...Argt >
	int cgl::launch(const Func & func, const Argt & ... args)
{
	boot();
	out.fresh();
	try
	{
		func(args...);
		exit();
		return 0;
	}
	catch(...)
	{
		out.destroy();
#ifdef CGL_TRACK
		cgl::track::caution_msg.log_to_file("CovGL_Cautions.log");
		cgl::track::error_msg.log_to_file("CovGL_Errors.log");
		cgl::track::stop("The specific reason is placed in the output file.");
#else
		cgl::track::stop("Please turn on Covariant GL Track Mod.");
#endif
	}
	return 1;
}


template < typename Ret, typename Func, typename...Argt >
	int cgl::launch(Ret & rv, const Func & func, const Argt & ... args)
{
	boot();
	out.fresh();
	try
	{
		rv = func(args...);
		exit();
		return 0;
	}
	catch(...)
	{
		out.destroy();
#ifdef CGL_TRACK
		cgl::track::caution_msg.log_to_file("CovGL_Cautions.log");
		cgl::track::error_msg.log_to_file("CovGL_Errors.log");
		cgl::track::stop("The specific reason is placed in the output file.");
#else
		cgl::track::stop("Please turn on Covariant GL Track Mod.");
#endif
	}
	return 1;
}


#endif