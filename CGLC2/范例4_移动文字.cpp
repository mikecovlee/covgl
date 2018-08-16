#include"CGL/Console"
int cglc_main() {
  cglc::screen scr;
  cglc::dynamic_text txt(scr);
  txt.back = cglc::black;
  while (true) {
    scr.clean();
    txt = "Hello!";
    for (int i = 0; i != scr.high() - 1; ++i)
      ++txt.y;
    for (int i = 0; i != scr.width() - txt.size(); ++i)
      ++txt.x;
    for (int i = 0; i != scr.high() - 1; ++i)
      --txt.y;
    for (int i = 0; i != scr.width() - txt.size(); ++i)
      --txt.x;
  }
}