#include"CGI.cpp"
#include"CGI_Views.cpp"
int cgi_main() {
  cgi::screen scr;
  pencil pen;
  pen.nib.img = ' ';
  pen.nib.b_color = "47";
  bool tst = false;
  while (true) {
    if (tst) {
      pen.nib.b_color = "40";
      tst = false;
    } else {
      pen.nib.b_color = "47";
      tst = true;
    }
    for (pen.y = 0; pen.y < scr.high(); ++pen.y)
      for (pen.x = 0; pen.x < scr.width(); ++pen.x) {
        scr << pen;
        scr.updscr();
      }
  }
}