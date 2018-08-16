#ifndef GUARD_CGL_Console_definer_h
#define GUARD_CGL_Console_definer_h
#include<iostream>
#include<thread>
#include<string>
#include<vector>
#include<time.h>
#include<termios.h>
#include<sys/ioctl.h>
#include<unistd.h>
// CGL for Console Class&Function definer
// Written by Mike Lee
namespace cglc {
  static const bool copyright = true;
  static const std::string foreground = "3";
  static const std::string background = "4";
  static const std::string black = "0";
  static const std::string red = "1";
  static const std::string green = "2";
  static const std::string yellow = "3";
  static const std::string blue = "4";
  static const std::string magenta = "5";
  static const std::string cyan = "6";
  static const std::string white = "7";
  static int automatic;
  static const int crazy_slow = 1000000;
  static const int slow = 18000;
  static const int normal = 14000;
  static const int fast = 10000;
  static const int max = 6000;
  static const int crazy_fast = 10;
  void move(int, int);
  void clrscr();
  class univerEvent;
    template < class > class classEvent;
    template < class > class c_position;
  class c_color;
  class c_point;
  class buffer;
  class screen;
  class baseStaticView;
  class baseDynamicView;
  class c_static_text;
  class c_dynamic_text;
    template < class T > using position = c_position < T >;
  typedef c_color color;
  typedef c_point point;
  typedef c_static_text static_text;
  typedef c_dynamic_text dynamic_text;
  static std::vector < buffer * >buffers;
  static std::vector < screen * >screens;
}
#endif