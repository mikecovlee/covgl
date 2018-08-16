#ifndef GUARD_CGL_Console_Main
#define GUARD_CGL_Console_Main
#include"HeadFiles/DEFINER.h"
// Covariate Graphics Library for Console
// T&C Studio出品
// 李登淳编写
// Copyright 2015 (C) T&C Studio
// Version 2.2.0
// Alpha 1
// Build 15-05-220-01
/* 类型定义 */
class cglc::univerEvent {
  void (*ctrl) ();
  bool active;
public:
  univerEvent() {
    active = false;
  };
  univerEvent(void (*func) ()) {
    ctrl = func;
    active = true;
  }
  void bind(void (*func) ()) {
    ctrl = func;
    active = true;
  }
  void touch_off() {
    if (active)
      ctrl();
  }
};
template < class T > class cglc::classEvent {
  void (T::*ctrl) ();
  bool active;
public:
  classEvent() {
    active = false;
  };
  classEvent(void (T::*func) ()) {
    ctrl = func;
    active = true;
  }
  void bind(void (T::*func) ()) {
    ctrl = func;
    active = true;
  }
  void touch_off(T * in) {
    if (active)
      (in->*ctrl) ();
  }
};
template < class T > class cglc::c_position {
  int m_position;
  bool active;
  T *object;
public:
  classEvent < T > onPositionMove;
  c_position() {
    m_position = 0;
    active = false;
  }
  void bind(T & binding) {
    object = &binding;
    active = true;
  }
  void move(int in) {
    m_position = in;
    if (active)
      onPositionMove.touch_off(object);
  }
  int position() {
    return m_position;
  }
  int operator++() {
    ++m_position;
    if (active)
      onPositionMove.touch_off(object);
    return m_position;
  }
  int operator--() {
    --m_position;
    if (active)
      onPositionMove.touch_off(object);
    return m_position;
  }
};
class cglc::c_color {
  std::string m_surface, m_color;
  bool compare(c_color);
public:
  c_color() {
    m_surface = foreground;
    m_color = white;
  }
  std::string & operator=(std::string in) {
    m_color = in;
    return this->m_color;
  }
  void set_surface(std::string in) {
    m_surface = in;
  }
  bool operator==(const c_color & in) {
    return compare(in);
  }
  bool operator!=(const c_color & in) {
    return !compare(in);
  }
  const std::string color() {
    return m_surface + m_color;
  }
};
class cglc::c_point {
  char img;
  bool compare(c_point);
public:
  c_color text, back;
  c_point() {
    img = ' ';
    text.set_surface(foreground);
    text = white;
    back.set_surface(background);
    back = black;
  }
  explicit c_point(char ch) {
    img = ch;
    text.set_surface(foreground);
    text = white;
    back.set_surface(background);
    back = black;
  }
  char &operator=(char tmp) {
    img = tmp;
    return this->img;
  }
  bool operator==(const c_point & in) {
    return compare(in);
  }
  bool operator!=(const c_point & in) {
    return !compare(in);
  }
  const char &image() {
    return this->img;
  }
};
class cglc::buffer {
  bool iuse;
  c_point initnib;
  int wid, hig;
  std::vector < std::vector < c_point >> paper;
  void create();
  void uncreate();
public:
  c_color back;
  buffer() {
    back.set_surface(background);
    back = black;
    iuse = false;
    create();
  }
  ~buffer() {
    uncreate();
  }
  bool reset();
  void init(int, int);
  bool use() {
    return iuse;
  }
  bool draw(int x, int y, c_point & tmp) {
    if (!iuse || x < 0 || y < 0 || x >= wid || y >= hig)
      return false;
    paper[y][x] = tmp;
    return true;
  }
  bool draw(int x, int y, char ch) {
    if (!iuse || x < 0 || y < 0 || x >= wid || y >= hig)
      return false;
    c_point tmp(ch);
    tmp.back = back;
    paper[y][x] = tmp;
    return true;
  }
  c_point & read(int x, int y) {
    return paper[y][x];
  }
  int width() {
    return wid;
  }
  int high() {
    return hig;
  }
};
class cglc::screen {
protected:
  int wid, hig;
  void create();
  void uncreate();
public:
  c_color back;
  int refresh = automatic;
  buffer renderer;
  screen();
  ~screen() {
    uncreate();
  }
  screen & operator<<(baseStaticView &);
  bool draw(int, int, buffer &);
  int width() {
    return wid;
  }
  int high() {
    return hig;
  }
  void clean() {
    renderer.back = back;
    renderer.reset();
  }
};
class cglc::baseStaticView {
protected:
  int wid, hig;
  buffer renderer;
public:
  int x, y;
  baseStaticView() {
    x = y = 0;
    wid = hig = 1;
    renderer.init(wid, hig);
  }
  explicit baseStaticView(int t_wid, int t_hig) {
    x = y = 0;
    wid = t_wid;
    hig = t_hig;
    renderer.init(wid, hig);
  }
  virtual bool use() {
    return renderer.use();
  }
  virtual const buffer & draw() {
    return renderer;
  }
};
class cglc::baseDynamicView {
protected:
  int wid, hig;
  buffer renderer;
  screen *object;
  virtual buffer & draw() {
    return renderer;
  }
  void refresh() {
    object->draw(x.position(), y.position(), draw());
  }
public:
  c_position < baseDynamicView > x, y;
  baseDynamicView() {
    x.move(0);
    y.move(0);
    x.bind(*this);
    y.bind(*this);
    x.onPositionMove.bind(&baseDynamicView::refresh);
    y.onPositionMove.bind(&baseDynamicView::refresh);
  }
  explicit baseDynamicView(screen & init) {
    object = &init;
    x.move(0);
    y.move(0);
    x.bind(*this);
    y.bind(*this);
    wid = hig = 1;
    renderer.init(wid, hig);
    x.onPositionMove.bind(&baseDynamicView::refresh);
    y.onPositionMove.bind(&baseDynamicView::refresh);
  }
  explicit baseDynamicView(int t_wid, int t_hig, screen & init) {
    object = &init;
    x.move(0);
    y.move(0);
    x.bind(*this);
    y.bind(*this);
    wid = t_wid;
    hig = t_hig;
    renderer.init(wid, hig);
    x.onPositionMove.bind(&baseDynamicView::refresh);
    y.onPositionMove.bind(&baseDynamicView::refresh);
  }
  virtual bool use() {
    return renderer.use();
  }
};
#include"HeadFiles/IO.h"
// 常用控件
class cglc::c_static_text:public baseStaticView {
  std::string str;
  void clean() {
    for (int m = 0; m != str.size(); ++m)
      if (str[m] == '\n')
        str[m] = '\0';
   }
public:
  c_color text, back;
  c_static_text() {
    x = y = 0;
    text.set_surface(foreground);
    back.set_surface(background);
  }
  explicit c_static_text(const std::string txt) {
    x = y = 0;
    text.set_surface(foreground);
    back.set_surface(background);
    str = txt;
    clean();
    renderer.init(str.size(), 1);
  }
  const buffer & draw() {
    cglc::c_point tmp;
    tmp.text = text;
    tmp.back = back;
    for (int n = 0; n != str.size(); ++n) {
      tmp = str[n];
      renderer.draw(n, 0, tmp);
    }
    return renderer;
  }
  c_static_text & operator=(const std::string txt) {
    str = txt;
    clean();
    renderer.init(str.size(), 1);
    return *this;
  }
  size_t size() {
    return str.size();
  }
};
class cglc::c_dynamic_text:public cglc::baseDynamicView {
protected:
  std::string str;
  void clean() {
    for (int m = 0; m != str.size(); ++m)
      if (str[m] == '\n')
        str[m] = '\0';
  }
  cglc::buffer & draw() {
    cglc::c_point tmp;
    tmp.text = text;
    tmp.back = back;
    for (int n = 0; n != str.size(); ++n) {
      tmp = str[n];
      renderer.draw(n, 0, tmp);
    }
    return renderer;
  }
  void refresh() {
    object->draw(x.position(), y.position(), draw());
    device << (*object);
  }
public:
  cglc::c_color text, back;
  cglc::c_position < c_dynamic_text > x, y;
  cglc::classEvent < c_dynamic_text > onTextChange;
  c_dynamic_text(cglc::screen & init) {
    object = &init;
    x.move(0);
    y.move(0);
    x.bind(*this);
    y.bind(*this);
    x.onPositionMove.bind(&c_dynamic_text::refresh);
    y.onPositionMove.bind(&c_dynamic_text::refresh);
    onTextChange.bind(&c_dynamic_text::refresh);
    text.set_surface(cglc::foreground);
    back.set_surface(cglc::background);
  }
  explicit c_dynamic_text(const std::string txt, cglc::screen & init) {
    object = &init;
    x.move(0);
    y.move(0);
    x.bind(*this);
    y.bind(*this);
    x.onPositionMove.bind(&c_dynamic_text::refresh);
    y.onPositionMove.bind(&c_dynamic_text::refresh);
    onTextChange.bind(&c_dynamic_text::refresh);
    text.set_surface(cglc::foreground);
    back.set_surface(cglc::background);
    str = txt;
    clean();
    renderer.init(str.size(), 1);
    onTextChange.touch_off(this);
  }
  c_dynamic_text & operator=(const std::string txt) {
    str = txt;
    clean();
    renderer.init(str.size(), 1);
    onTextChange.touch_off(this);
    return *this;
  }
  void operator>>(std::string & in) {
    in = str;
  }
  size_t size() {
    return str.size();
  }
};
/* 函数体定义 */
void cglc::move(int x, int y) {
  std::cout << "\x1B[" << x << ";" << y << "f";
}
void cglc::clrscr() {
  std::cout << "\x1B[2J\x1B[0;0f";
}
bool cglc::c_color::compare(cglc::c_color in) {
  if ((this->m_color == in.m_color) && (this->m_surface == in.m_surface))
    return true;
  else
    return false;
}
bool cglc::c_point::compare(cglc::c_point in) {
  if ((this->text == in.text) && (this->back == in.back)
      && (this->img == in.img))
    return true;
  else
    return false;
}
void cglc::buffer::create() {
  cglc::buffers.push_back(this);
}
void cglc::buffer::uncreate() {
  std::vector < cglc::buffer * >::iterator it = cglc::buffers.begin();
  while (it != buffers.end()) {
    if (*it == this)
      buffers.erase(it);
    else
      ++it;
  }
}
bool cglc::buffer::reset() {
  if (!iuse)
    return false;
  initnib.back = back;
  for (int i = 0; i != hig; ++i)
    for (int k = 0; k != wid; ++k)
      paper[i][k] = initnib;
  return true;
}
void cglc::buffer::init(int w, int h) {
  iuse = true;
  initnib = ' ';
  wid = w;
  hig = h;
  paper.resize(hig);
  for (int i = 0; i != hig; ++i)
    paper[i].resize(wid);
  reset();
}
void cglc::screen::create() {
  cglc::screens.push_back(this);
}
void cglc::screen::uncreate() {
  std::vector < cglc::screen * >::iterator it = cglc::screens.begin();
  while (it != screens.end()) {
    if (*it == this)
      screens.erase(it);
    else
      ++it;
  }
}
cglc::screen::screen() {
  struct winsize size;
  ioctl(STDIN_FILENO, TIOCGWINSZ, &size);
  wid = size.ws_col;
  hig = size.ws_row - 1;
  renderer.init(wid, hig);
  back = renderer.back;
  create();
}
cglc::screen & cglc::screen::operator<<(baseStaticView & tmp) {
  if (!tmp.use() || tmp.x >= wid || tmp.y >= hig || tmp.x < 0 || tmp.y < 0)
    return *this;
  buffer temp = tmp.draw();
  int i, k, m, n;
  i = k = 0;
  m = tmp.y;
  n = tmp.x;
  while (m < hig && i < temp.high()) {
    while (n < wid && k < temp.width()) {
      renderer.draw(n, m, temp.read(k, i));
      ++n;
      ++k;
    }
    n = tmp.x;
    k = 0;
    ++m;
    ++i;
  }
  return *this;
}
bool cglc::screen::draw(int x, int y, buffer & tmp) {
  if (!tmp.use() || x < 0 || y < 0 || x >= wid || y >= hig)
    return false;
  int i, k, m, n;
  i = k = 0;
  m = y;
  n = x;
  while (m < hig && i < tmp.high()) {
    while (n < wid && k < tmp.width()) {
      renderer.draw(n, m, tmp.read(k, i));
      ++n;
      ++k;
    }
    n = x;
    k = 0;
    ++m;
    ++i;
  }
  return true;
}
/* 准备工作 */
void loadvalues() {
  clock_t start, finish;
  start = clock();
  int i = 0;
  finish = clock();
  while (((finish - start) / CLOCKS_PER_SEC) < 1) {
    finish = clock();
    ++i;
  }
  i = i / 1000;
  i = 12000 - i;
  cglc::automatic = i;
}
void loadprompt() {
  std::cout << "\33[?25l";
  if (cglc::copyright) {
    cglc::screen scr;
    scr.back = cglc::cyan;
    scr.clean();
    cglc::static_text line0("Load Covariate GL for Console");
    cglc::static_text line1("Compile With Standard C++");
    cglc::static_text line2("Copyright 2015 (C) T&C Studio");
    line0.back = line1.back = line2.back = scr.back;
    line0.x = scr.width() / 2 - line0.size() / 2;
    line0.y = scr.high() - 3;
    line1.x = scr.width() / 2 - line1.size() / 2;
    line1.y = scr.high() - 2;
    line2.x = scr.width() / 2 - line2.size() / 2;
    line2.y = scr.high() - 1;
    scr << line0 << line1 << line2;
    cglc::device << scr;
    usleep(2000000);
  }
}
int cglc_main();
int main() {
  std::thread load0(loadprompt);
  std::thread load1(loadvalues);
  load0.join();
  load1.detach();
  cglc::clrscr();
  return cglc_main();
}
#endif