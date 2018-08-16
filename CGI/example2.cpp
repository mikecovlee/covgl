#include"CGI.cpp"
#include"CGI_Views.cpp"
int cgi_main() {
  cgi::screen scr;
  pencil pen;
  pen.nib.img = ' ';
  pen.nib.b_color = "47";
  bool xj = true;
  bool yj = true;
  while (true) {
    irefresh = 13000;
    scr.clean();
    scr << pen;
    if (pen.x == 0)
      xj = true;
    if (pen.y == 0)
      yj = true;
    if (pen.x == scr.width() - 1)
      xj = false;
    if (pen.y == scr.high() - 1)
      yj = false;
    if (xj)
      ++pen.x;
    else
      --pen.x;
    if (yj)
      ++pen.y;
    else
      --pen.y;
    scr.updscr();
  }
}