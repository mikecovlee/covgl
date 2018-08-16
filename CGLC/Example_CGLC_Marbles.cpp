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
  win0.renderer.back = "44";
  win1.renderer.back = "42";
  win2.renderer.back = "41";
  win0.title = "Marble Window 1";
  win1.title = "Marble Window 2";
  win2.title = "Marble Window 3";
  win0.clean();
  win1.clean();
  win2.clean();
  static_text txt0("Ball");
  static_text txt1("Ball");
  txt0.nib.back = txt1.nib.back = win0.renderer.back;
  pen0.nib = ' ';
  pen1.nib = ' ';
  pen2.nib = ' ';
  pen0.nib.back = "47";
  pen1.nib.back = "47";
  pen2.nib.back = "47";
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
    win0.clean();
    win1.clean();
    win2.clean();
    scr.clean();
    win0 << pen0;
    win1 << pen1;
    win2 << pen2;
    txt0 = "X=" + int2str(pen0.x) + " Width=" + int2str(win0.width());
    txt1 = "Y=" + int2str(pen0.y) + " High=" + int2str(win0.high());
    txt0.nib.back = txt1.nib.back = win0.renderer.back;
    win0 << txt0;
    win0 << txt1;
    txt0 = "X=" + int2str(pen1.x) + " Width=" + int2str(win1.width());
    txt1 = "Y=" + int2str(pen1.y) + " High=" + int2str(win1.high());
    txt0.nib.back = txt1.nib.back = win1.renderer.back;
    win1 << txt0;
    win1 << txt1;
    txt0 = "X=" + int2str(pen2.x) + " Width=" + int2str(win2.width());
    txt1 = "Y=" + int2str(pen2.y) + " High=" + int2str(win2.high());
    txt0.nib.back = txt1.nib.back = win2.renderer.back;
    win2 << txt0;
    win2 << txt1;
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
    scr << win0;
    scr << win1;
    scr << win2;
    scr.updscr();
  }
}