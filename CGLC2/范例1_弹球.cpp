#include"CGL/Views"
#include<sstream>
std::string int2str(int n) {
  std::stringstream ss;
  std::string str;
  ss << n;
  ss >> str;
  return str;
}
int cglc_main() {
  cglc::screen scr;
  window win0(scr.width() * 0.5 - 1, scr.high());
  window win1(scr.width() * 0.5 - 1, scr.high() * 0.5 - 0.5);
  window win2(scr.width() * 0.5 - 1, scr.high() * 0.5 - 0.5);
  win1.x = scr.width() * 0.5 + 1;
  win2.x = win1.x;
  win2.y = scr.high() * 0.5 + 1;
  pencil pen0, pen1, pen2;
  win0.back = cglc::blue;
  win1.back = cglc::green;
  win2.back = cglc::red;
  win0.title = "Marble Window 1";
  win1.title = "Marble Window 2";
  win2.title = "Marble Window 3";
  cls_win();
  cglc::text txt0, txt1;
  txt0.back = txt1.back = win0.back;
  pen0.nib = pen1.nib = pen2.nib = ' ';
  pen0.nib.back = pen1.nib.back = pen2.nib.back = cglc::white;
  bool xj0 = true;
  bool xj1 = false;
  bool xj2 = true;
  bool yj0 = true;
  bool yj1 = true;
  bool yj2 = false;
  ++txt1.y;
  pen1.x = win1.width() - 1;
  pen2.y = win2.high() - 1;
  while (true) {
    scr.clean();
    cls_win();
    win0 << pen0;
    win1 << pen1;
    win2 << pen2;
    txt0 = "X=" + int2str(pen0.x) + " Width=" + int2str(win0.width());
    txt1 = "Y=" + int2str(pen0.y) + " High=" + int2str(win0.high());
    txt0.back = txt1.back = win0.back;
    win0 << txt0 << txt1;
    txt0 = "X=" + int2str(pen1.x) + " Width=" + int2str(win1.width());
    txt1 = "Y=" + int2str(pen1.y) + " High=" + int2str(win1.high());
    txt0.back = txt1.back = win1.back;
    win1 << txt0 << txt1;
    txt0 = "X=" + int2str(pen2.x) + " Width=" + int2str(win2.width());
    txt1 = "Y=" + int2str(pen2.y) + " High=" + int2str(win2.high());
    txt0.back = txt1.back = win2.back;
    win2 << txt0 << txt1;
    if (pen0.x == 0)
      xj0 = true;
    if (pen0.y == 0)
      yj0 = true;
    if (pen0.x == win0.width() - 1)
      xj0 = false;
    if (pen0.y == win0.high() - 1)
      yj0 = false;
    if (pen1.x == 0)
      xj1 = true;
    if (pen1.y == 0)
      yj1 = true;
    if (pen1.x == win1.width() - 1)
      xj1 = false;
    if (pen1.y == win1.high() - 1)
      yj1 = false;
    if (pen2.x == 0)
      xj2 = true;
    if (pen2.y == 0)
      yj2 = true;
    if (pen2.x == win2.width() - 1)
      xj2 = false;
    if (pen2.y == win2.high() - 1)
      yj2 = false;
    if (xj0)
      ++pen0.x;
    else
      --pen0.x;
    if (yj0)
      ++pen0.y;
    else
      --pen0.y;
    if (xj1)
      ++pen1.x;
    else
      --pen1.x;
    if (yj1)
      ++pen1.y;
    else
      --pen1.y;
    if (xj2)
      ++pen2.x;
    else
      --pen2.x;
    if (yj2)
      ++pen2.y;
    else
      --pen2.y;
    scr << win0 << win1 << win2;
    cglc::device << scr;
  }
}