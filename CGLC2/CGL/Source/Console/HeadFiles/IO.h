#ifndef GUARD_CGL_Console_io_h
#define GUARD_CGL_Console_io_h
#include<iostream>
// CGL for Console Input/Output Headfile
// Written by Mike Lee
// Version 1.0.0
namespace cglc
{
class c_io
{
    void draw(screen &);
    buffer cache;
public:
    c_io & operator<<(screen &);
    c_io & operator>>(screen &);
};
typedef c_io io;
static io device;
}
void cglc::c_io::draw(screen & in)
{
    move(0, 0);
    for (int i = 0; i != in.renderer.high(); ++i)
    {
        for (int k = 0; k != in.renderer.width(); ++k)
        {
            if (in.renderer.read(k, i).image() == '\n')
                continue;
            std::cout << "\e[" << in.renderer.read(k, i).text.color() << ";1m";
            std::cout << "\e[" << in.renderer.read(k, i).back.color() << ";1m";
            std::cout << in.renderer.read(k, i).image();
        }
        std::cout << "\e[40;1m" << std::endl;
    }
    usleep(in.refresh);
}
cglc::c_io & cglc::c_io::operator<<(screen & in)
{
    cache = in.renderer;
    draw(in);
    return *this;
}
cglc::c_io & cglc::c_io::operator>>(screen & in)
{
    if (!cache.use())
        return *this;
    in.back = cache.back;
    in.clean();
    in.renderer = cache;
    return *this;
}
#endif