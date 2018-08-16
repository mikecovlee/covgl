#ifndef GUARD_CGI_H
#define GUARD_CGI_H
// Console Graphics Interface
// T&C Studio出品
// 李登淳编写
// Copyright 2015 (C) T&C Studio
// Version 2.0.0 Formal
// Build 15-04-06-200-2
// Notice:Don't use UTF Chars!Only support English.
#include<iostream>
#include<ios>
#include<string>
#include<vector>
#include<cstddef>
#include<termios.h>
#include<sys/types.h>
#include<sys/ioctl.h>
#include<unistd.h>
void gotoxy(int x, int y) {
  std::cout << "\x1B[" << x << ";" << y << "f";
}
namespace cgi {
  class point {
  public:
    char img;
      std::string t_color;
      std::string b_color;
      point() {
      img = '@';
      t_color = "37";
      b_color = "40";
  }};
  class buffer {
    friend class screen;
    bool use;
    point init_p;
    int width, high;
      std::vector < std::vector < point >> i_buffer;
  public:
      buffer() {
      use = false;
    } void init(int, int);
    void clean();
    std::vector < point > &operator[](size_t i) {
      return i_buffer[i];
    }
    size_t size() {
      return i_buffer.size();
    }
  };
  class baseStaticView {
    friend class screen;
  protected:
    int m_wid, m_hig;
    buffer renderer;
  public:
    int x, y;
      baseStaticView() {
      x = y = 0;
      m_wid = m_hig = 1;
      renderer.init(m_wid, m_hig);
    } baseStaticView(int t_wid, int t_hig) {
      x = y = 0;
      m_wid = t_wid;
      m_hig = t_hig;
      renderer.init(m_wid, m_hig);
    }
    virtual buffer draw() {
      return renderer;
    }
  };
  class screen {
    friend bool operator<<(baseStaticView &, baseStaticView &);
    int wid, hig;
  public:
      buffer renderer;
      screen();
      screen(int, int);
    bool operator<<(baseStaticView &);
    void updscr(bool);
    int width() {
      return wid;
    } int high() {
      return hig;
    }
    void clean() {
      renderer.clean();
      updscr(false);
    }
  };
}
#endif