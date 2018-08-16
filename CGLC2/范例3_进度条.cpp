#include"CGL/Views"
#include<sstream>
std::string int2str(int n) {
  std::stringstream ss;
  std::string str;
  ss << n;
  ss >> str;
  return str;
}
using namespace cglc;
int cglc_main() {
  screen scr;
  window win(scr);
  win.title = "Waiting...";
  scr.back = win.back = blue;
  scr.clean();
  win.clean();
  scr << win;
  device << scr;
  std::string proc(win.width() - 2, '=');
  std::string pers(int2str(0) + "%");
  static_text process(proc);
  static_text persent(pers);
  process.x = 1;
  process.y = 1;
  process.back = green;
  persent.back = blue;
  persent.x = win.width() / 2 - persent.size() / 2;
  persent.y = 2;
  win << process;
  scr << win;
  device << scr;
  scr.refresh = slow * 5;
  while (true) {
    for (int i = 0; i != proc.size(); ++i) {
      scr.clean();
      win.clean();
      proc[i] = '>';
      process = proc;
      double proce = (i * 100) / proc.size();
      persent = int2str(proce) + "%";
      persent.x = win.width() / 2 - persent.size() / 2;
      win << process << persent;
      scr << win;
      device << scr;
    }
    for (int i = proc.size(); i != 0; --i) {
      scr.clean();
      win.clean();
      proc[i - 1] = '=';
      process = proc;
      double proce = (i * 100) / proc.size();
      persent = int2str(proce) + "%";
      persent.x = win.width() / 2 - persent.size() / 2;
      win << process << persent;
      scr << win;
      device << scr;
    }
    for (int i = proc.size(); i != 0; --i) {
      scr.clean();
      win.clean();
      proc[i - 1] = '<';
      process = proc;
      double proce = (i * 100) / proc.size();
      persent = int2str(proce) + "%";
      persent.x = win.width() / 2 - persent.size() / 2;
      win << process << persent;
      scr << win;
      device << scr;
    }
    for (int i = 0; i != proc.size(); ++i) {
      scr.clean();
      win.clean();
      proc[i] = '=';
      process = proc;
      double proce = (i * 100) / proc.size();
      persent = int2str(proce) + "%";
      persent.x = win.width() / 2 - persent.size() / 2;
      win << process << persent;
      scr << win;
      device << scr;
    }
  }
}