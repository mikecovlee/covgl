#include "cgl_linux.cpp"
#include <sstream>
// Linux终端键盘I/O
bool KbHit()
{
	bool ret;
	fd_set fds;
	struct timeval tv;
	tv.tv_sec = 0;
	tv.tv_usec = 0;
	FD_ZERO(&fds);
	FD_SET(0, &fds);
	select(1, &fds, 0, 0, &tv);
	ret = FD_ISSET(0, &fds);
	return ret;
}
int GetKbHit()
{
	int ret;
	ret = getchar();
	return ret;
}
// 转换为字符串函数
template<typename T>
std::string toString(T val)
{
	std::stringstream ss;
	std::string ret;
	ss<<val;
	ss>>ret;
	return ret;
}
int main()
{
	using namespace cov::gl;
	// 初始化和设置
	std::cout<<"Console Pinball Game"<<std::endl;
	std::cout<<"Please set the difficulty.\nThe smaller your Settings,game play will be more difficult."<<std::endl;
	int jump_frame(4);
	std::cin>>jump_frame;
	ioctrl.init_output_method();
	char sbuf[512];
	fgets(sbuf,512,stdin);
	std::cout<<"Please set the \"up\" key.Default key is \'A\'.Press enter to skip."<<std::endl;
	char key_up=GetKbHit();
	if(key_up=='\n')
		key_up='a';
	std::cout<<"Please set the \"down\" key.Default key is \'Z\'.Press enter to skip."<<std::endl;
	char key_down=GetKbHit();
	if(key_down=='\n')
		key_down='z';
	int num3[]= {
		1,1,1,1,1,1,
		0,0,0,0,1,1,
		1,1,1,1,1,1,
		0,0,0,0,1,1,
		1,1,1,1,1,1
	};
	int num2[]= {
		1,1,1,1,1,1,
		0,0,0,0,1,1,
		1,1,1,1,1,1,
		1,1,0,0,0,0,
		1,1,1,1,1,1
	};
	int num1[]= {
		0,0,1,1,0,0,
		0,0,1,1,0,0,
		0,0,1,1,0,0,
		0,0,1,1,0,0,
		0,0,1,1,0,0
	};
	int chG[]= {
		1,1,1,1,1,1,
		1,1,0,0,0,0,
		1,1,0,1,1,1,
		1,1,0,0,1,1,
		1,1,1,1,1,1
	};
	int chO[]= {
		1,1,1,1,1,1,
		1,1,0,0,1,1,
		1,1,0,0,1,1,
		1,1,0,0,1,1,
		1,1,1,1,1,1
	};
	int chExc[]= {
		0,1,1,1,1,0,
		0,1,1,1,1,0,
		0,0,1,1,0,0,
		0,0,0,0,0,0,
		0,0,1,1,0,0
	};
	// 显示开始的倒计时
	ioctrl.frame_limit(1);
	image img(basic_io::get_target_width(),basic_io::get_target_height());
	auto draw_ch=[&](int* ch,int w,int h,int x,int y) {
		for(int r=0; r<h; ++r) {
			for(int c=0; c<w; ++c) {
				if(ch[r*6+c]==1)
					img.draw({c+x,r+y},pixel({true,false}, {colors::red,colors::red},' '));
			}
		}
	};
	img.fill(pixel({true,false}, {colors::white,colors::white},' '));
	draw_ch(num3,6,5,0.5*img.width()-3,0.5*img.height()-2.5);
	ioctrl.update_image(img);
	img.fill(pixel({true,false}, {colors::white,colors::white},' '));
	draw_ch(num2,6,5,0.5*img.width()-3,0.5*img.height()-2.5);
	ioctrl.update_image(img);
	img.fill(pixel({true,false}, {colors::white,colors::white},' '));
	draw_ch(num1,6,5,0.5*img.width()-3,0.5*img.height()-2.5);
	ioctrl.update_image(img);
	img.fill(pixel({true,false}, {colors::white,colors::white},' '));
	draw_ch(chG,6,5,0.5*img.width()-10,0.5*img.height()-2.5);
	draw_ch(chO,6,5,0.5*img.width()-2,0.5*img.height()-2.5);
	draw_ch(chExc,6,5,0.5*img.width()+5,0.5*img.height()-2.5);
	ioctrl.update_image(img);
	// 主循环
	ioctrl.frame_limit(60);
	int score=0;
	int zkh=0.5*img.height();
	int x(2),y(0),z1(0.5*img.height()-0.5*zkh),z2(0.5*img.height()-0.5*zkh);
	bool xj(true),yj(true);
	int delay_frame(0);
	while(true) {
		img.fill(pixel({true,false}, {colors::white,colors::blue},' '));
		for(int i=z1; i<=z1+zkh; ++i)
			img.draw({0,i},pixel({true,false}, {colors::cyan,colors::cyan},'#'));
		for(int i=z2; i<=z2+zkh; ++i)
			img.draw({img.width()-1,i},pixel({true,false}, {colors::magenta,colors::magenta},'#'));
		if(delay_frame>jump_frame) {
			delay_frame=0;
			if(x<=1) {
				if(y<z1 || y>z1+zkh)
					break;
				xj=true;
				x=1;
				++score;
			}
			if(y<=0) {
				yj=true;
				y=0;
			}
			if(x>=img.width()-2) {
				if(y<z2 || y>z2+zkh)
					break;
				xj=false;
				x=img.width()-2;
				++score;
			}
			if(y>=img.height()-1) {
				yj=false;
				y=img.height()-1;
			}
			if(xj)
				++x;
			else
				--x;
			if(yj)
				++y;
			else
				--y;
		} else
			++delay_frame;
		if(KbHit()) {
			char in=GetKbHit();
			if(in==key_up) {
				if(xj) {
					if(z2>0) --z2;
				} else {
					if(z1>0) --z1;
				}
			}
			if(in==key_down) {
				if(xj) {
					if(z2+zkh<img.height()-1) ++z2;
				} else {
					if(z1+zkh<img.height()-1)++z1;
				}
			}
		}
		img.draw({x,y},pixel({true,false}, {colors::red,colors::yellow},'@'));
		std::string info="FPS:"+toString(ioctrl.fps())+" Score:"+toString(score);
		for(int i=0; i<info.size(); ++i) {
			img.draw({i,0},pixel({true,false}, {colors::white,colors::black},info[i]));
		}
		ioctrl.update_image(img);
	}
	// 结束后处理得分
	ioctrl.stop_output_method();
	std::cout<<"You Die!Your Score is "<<score<<'.'<<std::endl;
	if(score<5)
		std::cout<<"Very Bad.Just try lower difficulty."<<std::endl;
	if(score>=5&&score<10)
		std::cout<<"Not Bad.You can get higher score."<<std::endl;
	if(score>=10&&score<50)
		std::cout<<"Good."<<std::endl;
	if(score>=50&&score<100)
		std::cout<<"Very Great!"<<std::endl;
	if(score>=100)
		std::cout<<"So Amazing!"<<std::endl;
	return 0;
}