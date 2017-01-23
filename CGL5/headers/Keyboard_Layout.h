#ifndef CGL_KEYBOARD_LAYOUT_H


#define CGL_KEYBOARD_LAYOUT_H


namespace cgl
{
	namespace keyboard
	{
#if defined(CGL_KBLAYOUT_PHONE)
		const char num_up = '2';
		const char num_down = '8';
		const char num_left = '4';
		const char num_right = '6';
#else
#if defined(CGL_KBLAYOUT_PC)
		const char num_up = '8';
		const char num_down = '2';
		const char num_left = '4';
		const char num_right = '6';
#endif
#endif
		const char num_select = '5';
		const int tab = 9;
		const int enter = 10;
		const int esc = 27;
		const int back_space = 127;
	}
}


#endif