#include"CGL/Views"
int cglc_main() {
  using namespace std;
  using namespace cglc;
  screen scr;
  scr.renderer.back = "47";
  scr.clean();
  c_point stone, grass, dirt, suns, per;
  stone = '#';
  stone.text = "30";
  stone.back = "44";
  grass = '#';
  grass.text = "32";
  grass.back = "42";
  dirt = '#';
  dirt.text = "33";
  dirt.back = "41";
  suns = ' ';
  suns.text = "33";
  suns.back = "43";
  per = '#';
  per.text = "30";
  per.back = "47";
  buffer renderer;
  renderer.init(scr.width(), scr.high());
  renderer.back = "47";
  renderer.reset();
  int x, y;
  for (y = renderer.high() - 1; y > 0.9 * renderer.high(); --y)
    for (x = 0; x < renderer.width(); ++x)
      renderer.draw(x, y, stone);
  for (; y > 0.7 * renderer.high(); --y)
    for (x = 0; x < renderer.width(); ++x)
      renderer.draw(x, y, dirt);
  for (; y > 0.65 * renderer.high(); --y)
    for (x = 0; x < renderer.width(); ++x)
      renderer.draw(x, y, grass);
  buffer sun;
  sun.init(4, 2);
  sun.draw(0, 0, suns);
  sun.draw(1, 0, suns);
  sun.draw(2, 0, suns);
  sun.draw(3, 0, suns);
  sun.draw(0, 1, suns);
  sun.draw(1, 1, suns);
  sun.draw(2, 1, suns);
  sun.draw(3, 1, suns);
  vector < string > perimg;
  perimg.resize(9);
  perimg[0] = " ### ";
  perimg[1] = " #*# ";
  perimg[2] = " ### ";
  perimg[3] = "  #  ";
  perimg[4] = " ### ";
  perimg[5] = "# # #";
  perimg[6] = "  #  ";
  perimg[7] = " # # ";
  perimg[8] = "#   #";
  buffer person;
  person.init(5, 9);
  person.back = "47";
  person.reset();
  for (int i = 0; i != 9; ++i)
    for (int k = 0; k != 5; ++k) {
      per = perimg[i][k];
      person.draw(k, i, per);
    }
  x = 0;
  y -= 8;
  scr.draw(0, 0, renderer);
  scr.draw(2, 1, sun);
  scr.draw(x, y, person);
  scr.updscr();
  int xm, ym;
  xm = ym = 0;
  bool jump = false;
  while (true) {
    char input;
    if (jump) {
      usleep(200000);
      input = '0';
      ym += 2;
      jump = false;
    } else
      cin >> input;
    switch (input) {
    case '0':
      break;
    case '1':
      xm -= 1;
      ym -= 1;
      break;
    case '2':
      ym -= 1;
      break;
    case '3':
      xm += 1;
      ym -= 1;
      break;
    case '4':
      xm -= 1;
      break;
    case '5':
      ym -= 2;
      jump = true;
      break;
    case '6':
      xm += 1;
      break;
    case '7':
      xm -= 1;
      ym += 1;
      break;
    case '8':
      ym += 1;
      break;
    case '9':
      xm += 1;
      ym += 1;
      break;
    }
    scr.clean();
    scr.draw(0, 0, renderer);
    scr.draw(2, 1, sun);
    scr.draw(x + xm, y + ym, person);
    scr.updscr();
  }
}