// CGI函数外部定义文件
// Build 15-04-06-200-2
#include"CGI.h"
static int irefresh = 11111;
void cgi::buffer::init(int w, int h) {
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
void cgi::buffer::clean() {
  if (!use)
    return;
  for (int i = 0; i != high; ++i)
    for (int k = 0; k != width; ++k)
      i_buffer[i][k] = init_p;
}
cgi::screen::screen() {
  struct winsize size;
  ioctl(STDIN_FILENO, TIOCGWINSZ, &size);
  wid = size.ws_col;
  hig = size.ws_row - 7;
  renderer.init(wid, hig);
}
cgi::screen::screen(int m_wid, int m_hig) {
  wid = m_wid;
  hig = m_hig;
  renderer.init(m_wid, m_hig);
}
bool cgi::screen::operator<<(baseStaticView & tmp) {
  if (!tmp.renderer.use || tmp.x >= wid || tmp.y >= hig || tmp.x < 0
      || tmp.y < 0)
    return false;
  buffer temp = tmp.draw();
  int i, k, m, n;
  i = k = 0;
  m = tmp.y;
  n = tmp.x;
  while (m < hig && i < temp.size()) {
    while (n < wid && k < temp[i].size()) {
      renderer[m][n] = temp[i][k];
      ++n;
      ++k;
    }
    ++m;
    ++i;
  }
  return true;
}
void cgi::screen::updscr(bool irfsh = true) {
  std::cout << std::flush << "\33[?25l";
  gotoxy(0, 0);
  for (int i = 0; i != renderer.i_buffer.size(); ++i) {
    for (int k = 0; k != renderer.i_buffer[i].size(); ++k) {
      if (renderer.i_buffer[i][k].img == '\n')
        continue;
      std::cout << "\e[" << renderer.i_buffer[i][k].t_color << ";1m\e[" <<
        renderer.i_buffer[i][k].b_color << ";1m" << renderer.i_buffer[i][k].
        img;
    }
    std::cout << std::endl;
  }
  if (irfsh)
    usleep(irefresh);
}
int cgi_main();
int main() {
  return cgi_main();
}