#include<iostream>
#include<string>
#include<vector>
#include<termios.h>
#include<sys/types.h>
#include<sys/ioctl.h>
#include<unistd.h>
class screen {
private:
  int c, r;
public:
    screen() {
    struct winsize size;
      ioctl(STDIN_FILENO, TIOCGWINSZ, &size);
      c = size.ws_col;
      r = size.ws_row;
  } int col() const {
    return c;
  }
  int row() const {
    return r;
  }
};
class windows {
private:
  int row, col;
  char image;
    std::vector < std::vector < char > >window;
  void d_row() {
    for (int j = 0; j != col + 2; ++j)
      std::cout << image;
  } void fill() {
    for (int i = 0; i != row; ++i)
      for (int j = 0; j != col; ++j)
        window[i][j] = ' ';
  }
  void clear() {
    std::cout << "\x1B[2J\x1B[0;0f";
  }
public:
  std::string title;
  windows() {
    screen scr;
    col = scr.col() - 2;
    row = scr.row() / 2;
    image = '#';
    window.resize(row);
    for (int i = 0; i != row; ++i)
      window[i].resize(col);
    fill();
    title="Console Window";
  }
  windows(int w, int h, char c_image) {
    col = w;
    row = h;
    image = c_image;
    window.resize(h);
    for (int i = 0; i != row; ++i)
      window[i].resize(w);
    fill();
    title="Console Window";
  }
  int width() {
    return col;
  }
  int high() {
    return row;
  }
  void clean() {
    clear();
    fill();
  }
  void display() {
    clear();
    std::cout << "\33[?25l";
    d_row();
    std::string space(col-title.size(),' ');
    std::string line(col,'-');
    std::cout<<image<<title<<space<<image<<std::endl;
    std::cout<<image<<line<<image<<std::endl;
    for (int i = 0; i != row; ++i) {
      std::cout << image;
      for (int j = 0; j != col; ++j)
        std::cout << window[i][j];
      std::cout << image;
    }
    std::cout << std::endl;
    d_row();
    std::cout << std::endl;
    usleep(50000);
  }
  bool draw_point(int x, int y, char c_image) {
    if (x > col || x == col || y > row || y == row)
      return false;
    window[y][x] = c_image;
    return true;
  }
};