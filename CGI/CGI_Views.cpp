// T&C Studio官方开发控件
// 适用于Build 15-04-xx-200-x
class pencil:public cgi::baseStaticView {
public:
  cgi::point nib;
  pencil() {
    x = y = 0;
    nib.img = '@';
    nib.t_color = "37";
    nib.b_color = "40";
    renderer.init(1, 1);
  }
  cgi::buffer draw() {
    renderer[0][0] = nib;
    return renderer;
  }
};
class static_text:public pencil {
    std::string str;
  void clean() {
    for (int m = 0; m != str.size(); ++m)
      if (str[m] == '\n')
        str[m] = '\0';
  }
  public:
    static_text(const std::string & txt) {
    x = y = 0;
    nib.img = '@';
    nib.t_color = "37";
    nib.b_color = "40";
    str = txt;
    clean();
    renderer.init(str.size(),1);
  }
  cgi::buffer draw() {
      for (int n = 0; n != str.size(); ++n) {
        nib.img = str[n];
        renderer[0][n] = nib;
      }
      nib.img = '@';
    return renderer;
  }
  void operator=(const std::string & txt) {
    str = txt;
    clean();
    renderer.init(str.size(),1);
  }
  size_t size() {
    return str.size();
  }
};