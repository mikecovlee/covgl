#include"CGL/Views"
int cglc_main() {
  cglc::screen scr;
  scr.renderer.back = "47";
  scr.clean();
  window win0(20, 10);
  window win1(20, 10);
  window win2(20, 10);
  win0.x = win0.y = 3;
  win1.x = win1.y = 6;
  win2.x = win2.y = 9;
  win0.renderer.back = "42";
  win1.renderer.back = "43";
  win2.renderer.back = "44";
  win0.clean();
  win1.clean();
  win2.clean();
  win2.title = "About Windows";
  scr << win0;
  scr << win1;
  scr << win2;
  std::string startmenu(scr.width(), ' ');
  static_text sm(startmenu);
  sm.y = scr.high() - 1;
  sm.nib.back = "44";
  scr << sm;
  static_text hl("#START");
  hl.nib.back = "42";
  hl.y = scr.high() - 1;
  scr << hl;
  static_text hw("\"Windows\" XP 2.0");
  hw.nib.back = "44";
  win2 << hw;
  scr << win2;
  scr.updscr();
}