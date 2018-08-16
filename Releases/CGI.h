#ifndef GUARD_CGI_H
#define GUARD_CGI_H
// Console Graphics Interface
// T&C Studio Present
// Compile By Mike Lee
// Copyright 2015 (C) T&C Studio
// Version 1.1.0 Formal
// Notice:Don't use UTF Chars!Only support English.
#include<iostream>
#include<string>
#include<vector>
#include<cstddef>
#include<termios.h>
#include<sys/types.h>
#include<sys/ioctl.h>
#include<unistd.h>
// Please use namespace cgi.
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
  class pencil {
  public:
    int x, y;
    point nib;
      pencil() {
      x = y = 0;
      nib.img = '@';
      nib.t_color = "37";
      nib.b_color = "40";
  }};
  class static_text:public pencil {
  private:
    friend class buffer;
    std::string str;
    void clean() {
      for (int m = 0; m != str.size(); ++m)
        if (str[m] == '\n')
          str[m] = '\0';
  } public:
      static_text(const std::string & txt) {
      x = y = 0;
      nib.img = '@';
      nib.t_color = "37";
      nib.b_color = "40";
      str = txt;
      clean();
    } void operator=(const std::string & txt) {
      str = txt;
      clean();
    }
    size_t size() {
      return str.size();
    }
  };
  class buffer {
    friend class screen;
    friend bool operator<<(pencil &, pencil &);
    friend bool operator<<(static_text &, static_text &);
  private:
      bool use;
    point init_p;
    int width, high;
      std::vector < std::vector < point >> i_buffer;
  public:
      buffer() {
      use = false;
    } void init(int w, int h) {
      use = true;
      init_p.img = ' ';
      width = w;
      high = h;
      i_buffer.resize(high);
      for (int i = 0; i != high; ++i)
        i_buffer[i].resize(width);
      for (int i = 0; i != high; ++i)
        for (int k = 0; k != width; ++k)
          i_buffer[i][k] = init_p;
    }
    bool operator<<(pencil & tmp) {
      if (!use || tmp.x < 0 || tmp.y < 0 || tmp.x >= width || tmp.y >= high
          || tmp.nib.img == '\n')
        return false;
      i_buffer[tmp.y][tmp.x] = tmp.nib;
      return true;
    }
    bool operator<<(static_text & tmp) {
      if (!use || tmp.x < 0 || tmp.y < 0 || tmp.x + tmp.size() >= width
          || tmp.y >= high)
        return false;
      for (int n = 0; n != tmp.size(); ++n) {
        tmp.nib.img = tmp.str[n];
        i_buffer[tmp.y][tmp.x + n] = tmp.nib;
      }
      tmp.nib.img = '@';
      return true;
    }
    void clean() {
      if (!use)
        return;
      for (int i = 0; i != high; ++i)
        for (int k = 0; k != width; ++k)
          i_buffer[i][k] = init_p;
    }
  };
  class screen {
  private:
    int wid, hig;
    void clear() {
      std::cout << "\x1B[2J\x1B[0;0f";
  } public:
      buffer renderer;
    screen() {
      struct winsize size;
      ioctl(STDIN_FILENO, TIOCGWINSZ, &size);
      wid = size.ws_col;
      hig = size.ws_row - 6;
      renderer.init(wid, hig);
    }
    screen(int m_wid, int m_hig) {
      wid = m_wid;
      hig = m_hig;
      renderer.init(m_wid, m_hig);
    }
    void update_scr() {
      std::cout << "\33[?25l";
      clear();
      for (int i = 0; i != renderer.i_buffer.size(); ++i) {
        for (int k = 0; k != renderer.i_buffer[i].size(); ++k) {
          if (renderer.i_buffer[i][k].img == '\n')
            continue;
          std::cout << "\e[" + renderer.i_buffer[i][k].t_color + ";1m\e[" +
            renderer.i_buffer[i][k].b_color + ";1m" +
            renderer.i_buffer[i][k].img;
        }
        std::cout << std::endl;
      }
      usleep(12000);
    }
    int width() {
      return wid;
    }
    int high() {
      return hig;
    }
    void clean() {
      renderer.clean();
      clear();
    }
  };
}
int cgi_main();
int main() {
  return cgi_main();
}
#endif