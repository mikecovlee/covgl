#include"CGL/Console"
#include <sys/select.h>
void terminal_lnbuf(int yn) {
   struct termios oldt, newt;
   tcgetattr(0, &oldt);
   newt = oldt;
   if (!yn) newt.c_lflag &= ~ICANON;
   else newt.c_lflag |= ICANON;
   tcsetattr(0, TCSANOW, &newt);
}
void terminal_echo(int yn) {
   struct termios oldt, newt;
   tcgetattr(0, &oldt);
   newt = oldt;
   if (!yn) newt.c_lflag &= ~ECHO;
   else newt.c_lflag |= ECHO;
   tcsetattr(0, TCSANOW, &newt);
}
int getch() {
   register int ch;
   terminal_lnbuf(0);
   terminal_echo(0);
   ch = getchar();
   terminal_lnbuf(1);
   terminal_echo(1);
   return ch;
}
int cglc_main() {
  cglc::screen scr;
  cglc::dynamic_text txt(scr);
  txt.back = cglc::black;
  std::vector < cglc::dynamic_text > txts;
  std::vector < std::string > swap;
  for (int i = 0; i != scr.high(); ++i) {
    txts.push_back(txt);
    txts[i].y.move(i);
  }
  std::string greet = "_";
  txts[0] = greet;
  int line = 0;
  while (true) {
    if (greet.size() >= scr.width()) {
      greet.erase(greet.size() - 1);
      swap.push_back(greet);
      ++line;
      greet = "_";
      continue;
    }
    greet.erase(greet.size() - 1);
    scr.clean();
    char in = getch();
    if (in == 10) {
      swap.push_back(greet + " ");
      ++line;
      greet = "_";
      for (int i = 0; i != swap.size(); ++i)
        txts[i] = swap[i];
      txts[line] = greet;
      continue;
    }
    if (in == 127) {
      if (greet.size() == 0 && line > 0) {
        --line;
        swap.erase(swap.end());
        txts[line] >> greet;
      }
      if (greet.size() > 0)
        greet.erase(greet.size() - 1);
    } else
      greet += in;
    greet += "_";
    for (int i = 0; i != swap.size(); ++i)
      txts[i] = swap[i];
    txts[line] = greet;
  }
}