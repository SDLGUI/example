#ifndef __plane_gui__
#define __plane_gui__
#include "hitbox_data.cpp"
#include "iostream"
#include "sdlgui.h"
#include <stdio.h>
using namespace std;

const int gw = 270;
const int gh = 270;

//-----------------------------------------
//
//		活动方块类
//
//-----------------------------------------
typedef class float_box_struct : public GUI<float_box_struct,sdl_widget>
{
	public:
		float_box_struct();
		float_box_struct(const char*,int,int,int,int,Uint32);
		int init();
		int init(const char*,int,int,int,int,Uint32);
		int sysevent(SDL_Event* e);
	public:
		int a,am;
}*float_box_struct_ptr;
float_box_struct::float_box_struct()
{
	init();
}
float_box_struct::float_box_struct(const char* ptitle,int px,int py,int pw,int ph,Uint32 pflag)
{
	init(ptitle,px,py,pw,ph,pflag);
}
int float_box_struct::init(const char* ptitle,int px,int py,int pw,int ph,Uint32 pflag)
{
	SDL_Rect rt;
	if(sdl_widget::init(ptitle,px,py,pw,ph,pflag))return -1;
	fill_rect(NULL,0x00ff00);
	add_timer(100);
	am = 1;
	a = 255;
	blend(SDL_BLENDMODE_BLEND);
	return 0;
}
int float_box_struct::init()
{
	return sdl_widget::init();
}
int float_box_struct::sysevent(SDL_Event *e)
{
	switch(e->type)
	{
		case SDL_USEREVENT:
			switch(e->user.code)
			{
				case sdlgui_event_timer:
					if(am)
					{
						a /=2;
						if(a <=60 )am = 0;
					}
					else
					{
						a*=2;
						if(a>=255)am=1;
					}
					alpha(a);
					//cout<<a<<endl;
				break;
			}
		break;
	}
}

//----------------------------------------------
//
//			填充方块类
//
//----------------------------------------------
typedef class push_box_struct : public GUI<push_box_struct,sdl_widget>
{
	public:
		push_box_struct();
		push_box_struct(const char*,int,int,int,int,Uint32);
		int init(const char*,int,int,int,int,Uint32);
		int init();
		int push(int,int);
		int pull();
		int sysevent(SDL_Event *);
	public:
		int _is_push;
		SDL_TimerID tid;
		int tx,ty;
}*push_box_struct_ptr;
push_box_struct::push_box_struct()
:
GUI<push_box_struct,sdl_widget>()
{
	init();
}
push_box_struct::push_box_struct(const char* ptitle,int px,int py,int pw,int ph,Uint32 pflags)
:
GUI<push_box_struct,sdl_widget>()
{
	init(ptitle,px,py,pw,ph,pflags);
}
int push_box_struct::init()
{
	if(sdl_widget::init())return -1;
	_is_push = 0;
	tx = 800;
	ty = 800;
	return 0;
}
int push_box_struct::init(const char* ptitle,int px,int py,int pw,int ph,Uint32 pflags)
{
	if(sdl_widget::init(ptitle,px,py,pw,ph,pflags))return -1;
	blend(SDL_BLENDMODE_BLEND);
	tid = add_timer(100);
	return 0;
}
int push_box_struct::push(int x,int y)
{
	_is_push = 1;
	alpha(_is_push);
	tx = x;
	ty = y;
	return pos(x,y);
}
int push_box_struct::pull()
{
	_is_push = 0;
	tx = 800;
	ty = 800;
	return pos(800,800);
}
int push_box_struct::sysevent(SDL_Event* e)
{
	static int x = -1;
	static int y = -1;
	switch(e->type)
	{
		case SDL_USEREVENT:
			switch(e->user.code)
			{
				case sdlgui_event_timer:
					if(_is_push && _is_push<=255)
					{
						_is_push*=5;
						if(_is_push<255)
						{
							x *= (255-_is_push)/100*-1;
							pos(tx+x,ty);
						}
						else
						{
							x = -1;
							y = 1;
							_is_push=255;
							pos(tx,ty);
						}
						alpha(_is_push);
					}
				break;
			}
		break;
	}
}
//-----------------------------------------
//
//		方块盘类
//
//-----------------------------------------
typedef class plane_gui : public GUI<plane_gui,sdl_widget>
{
	public:
		plane_gui();
		plane_gui(const char*,int,int,int,int,Uint32);
		int init();
		int init(const char*,int,int,int,int,Uint32);
		int start(int);
		int sysevent(SDL_Event *e);
	public:
		int sw,sh;
		hitbox_plane_struct plane;
		float_box_struct_ptr f;
		push_box_struct_ptr b[3];
		push_box_struct_ptr bt[3];
		push_box_struct_ptr pt[box_width][box_height];
		sdl_widget_ptr state_show;
		sdltext_ptr state_show_text;
		SDL_Rect box_rt;
		sdlsurface bg;
		sdl_clip clip;
}*plane_gui_ptr;
plane_gui::plane_gui()
:
GUI<plane_gui,sdl_widget>()
{
	init();
}
plane_gui::plane_gui(const char* ptitle,int px,int py,int pw,int ph,Uint32 pflag)
:
GUI<plane_gui,sdl_widget>()
{
	init(ptitle,px,py,pw,ph,pflag);
}
int plane_gui::init(const char* ptitle,int px,int py,int pw,int ph,Uint32 pflag)
{
	int x,y;
	SDL_Rect rt;
	/* 每个小方块大小 */
	box_rt.w = pw/box_width;
	box_rt.h = ph/box_height;
	//
	init();
	bg.img_load("bg.jpg");
	clip.init(&bg,box_rt.w,box_rt.h);
	if(sdl_widget::init(ptitle,px,py,pw,ph,pflag))return -1;
	/* 生成活动方块 */
	f = add<float_box_struct>("",0,0,box_rt.w,box_rt.h,1);
	clip(0,0)->blit_surface(NULL,f,NULL);
	/* 生成将填充方块 */
	bt[0] = add<push_box_struct>("",pw,ph,box_rt.w,box_rt.h,1);
	bt[0]->fill_rect(NULL,0x00ff00);
	bt[1] = add<push_box_struct>("",pw,ph,box_rt.w,box_rt.h,1);
	bt[1]->fill_rect(NULL,0x00ff00);
	bt[2] = add<push_box_struct>("",pw,ph,box_rt.w,box_rt.h,1);
	bt[2]->fill_rect(NULL,0x00ff00);
	/* 生成状态面板 */
	state_show = add<sdl_widget>("",0,0,pw,ph,1);
	state_show->img_load("bg.jpg");
	state_show_text = new sdltext("c:/windows/fonts/simkai.ttf",64);
	state_show_text->render_utf8_solid("你胜利了！！！",0x00ff00);
	state_show_text->blit_surface(NULL,state_show,NULL);
	//state_show->fill_rect(NULL,0xff0000);

	//state_show->text("你成功了");
	start(0);
	return 0;
}
int plane_gui::init()
{
	return 0;
}
int plane_gui::start(int plevel)
{
	sdlsurface*tsur;
	plane.level(plevel);
	int x,y;
	SDL_Rect rt;
	/* 每个小方块大小 */
	box_rt.w = _rect.w/box_width;
	box_rt.h = _rect.h/box_height;
	fill_rect(NULL,0x000000);
	/* 绘制方块盘子 */
	for(y=0;y<box_height;y++)
	{
		for(x=0;x<box_width;x++)
		{
			pt[x][y] = NULL;
			box_rt.x = x*box_rt.w;
			box_rt.y = y*box_rt.h;
			tsur = clip(y,x);
			tsur->surface_blend_mode(SDL_BLENDMODE_BLEND);
			switch(plane.box_id(x,y))
			{
				case bind_box_id:
					//fill_rect(&box_rt,0x000000);
					tsur->surface_alpha_mod(255);
					tsur->blit_surface(NULL,this,&box_rt);
				break;
				case push_box_id:
					//fill_rect(&box_rt,0x00ffff);
					tsur->surface_alpha_mod(10);
					tsur->blit_surface(NULL,this,&box_rt);
				break;
			case empty_box_id:
					//fill_rect(&box_rt,0x0000ff);
					tsur->surface_alpha_mod(60);
					tsur->blit_surface(NULL,this,&box_rt);
				break;
				case pass_box_id:
					tsur->surface_alpha_mod(40);
					//fill_rect(&box_rt,0xff0000);
					tsur->blit_surface(NULL,this,&box_rt);
				break;
			}
		}
	}
	/*  */
	b[0] = bt[0];
	b[0]->pull();
	b[1] = bt[1];
	b[1]->pull();
	b[2] = bt[2];
	b[2]->pull();
	state_show->hide();
}
int plane_gui::sysevent(SDL_Event *e)
{
	int v;
	int x,y;
	switch(e->type)
	{
		case SDL_USEREVENT:
			if(plane.check())
			{
				start(plane.level());
			}
			else
			{
				start(plane.level()+1);
			}
		break;
		case SDL_KEYUP:
			switch(e->key.keysym.sym)
			{
				case SDLK_UP:
					v = plane.move(box_move_up);
				break;
				case SDLK_DOWN:
					v = plane.move(box_move_down);
				break;
				case SDLK_LEFT:
					v = plane.move(box_move_left);
				break;
				case SDLK_RIGHT:
					v = plane.move(box_move_right);
				break;
				defalut:
					return sdl_widget::sysevent(e);
				break;
			}
			/* 如果移动时出错 */
			if(v==-1)
			{
				cout<<"Error"<<endl;
			}
			else
			/* 移动方块 */
			if(v>0 && v<100)
			{
				clip(plane._float_box_y,plane._float_box_x)->blit_surface(NULL,f,NULL);
				f->pos(plane._float_box_x*box_rt.w,plane._float_box_y*box_rt.h);
				cout<<"move"<<endl;
			}
			else
			/* 摘取方块 */
			if(v>=100 && v<1000)
			{
				x = int((v-100)/10);
				y = v-100-x*10;
				//pt[x][y]->pos(600,600);
				pt[x][y]->pull();
				b[plane.count()-1] = pt[x][y];
				cout<<"pull"<<plane.count()<<":"<<x<<":"<<y<<endl;
			}
			else
			/* 填充方块 */
			{
				x = int((v-1000)/10);
				y = v - 1000 - x*10;
				pt[x][y] = b[plane.count()];
				pt[x][y]->push(x*box_rt.w,y*box_rt.h);
				//clip(y,x)->blit_surface(NULL,b[plane.count()],NULL);
				if(!plane.check())
				{
					//cout<<state_show<<endl;
					state_show->show();
				}
				cout<<"push:"<<x<<":"<<y<<endl;
			}
		break;
	}
	return sdl_widget::sysevent(e);
}
//-----------------------------------------
//
//			方块信息
//
//-----------------------------------------
typedef class box_info_struct : public GUI<box_info_struct,sdl_widget>
{
	public:
		box_info_struct();
		box_info_struct(const char*,int,int,int,int,Uint32);
		int init();
		int init(const char*,int,int,int,int,Uint32);
		int sysevent(SDL_Event*);
	public:
		int info(hitbox_plane_struct*);
		int time(int);
	public:
		char _buf[1000];
		int _box_count;
		int _time_total;
		int _time_current;
		int _level;
		//
		sdl_widget_ptr _box_text;
		sdl_button_ptr _start;
}*box_info_struct_ptr;
box_info_struct::box_info_struct()
:
GUI<box_info_struct,sdl_widget>()
{
	init();
}
box_info_struct::box_info_struct(const char* ptitle,int px,int py,int pw,int ph,Uint32 pflag)
{
	init(ptitle,px,py,pw,ph,pflag);
}
int box_info_struct::init()
{
	return sdl_widget::init();
}
int box_info_struct::init(const char* ptitle,int px,int py,int pw,int ph,Uint32 pflag)
{
	int global_color = 0xffffff;
	int text_height = 20;
	_time_current = 0;
	if(sdl_widget::init(ptitle,px,py,pw,ph,pflag))return -1;
	/* 加入开始按钮 */
	_start = add<sdl_button>("",240,10,30,text_height,1);
	_start->text("开始");
	/* 加入停息栏 */
	_box_text = add<sdl_widget>("",0,10,240,text_height,1);
	_box_text->fill_rect(NULL,global_color);
	add_timer(1000);
	return 0;
}
int box_info_struct::info(hitbox_plane_struct* p)
{
	/* 更新 */
	memset(_buf,0x00,1000);
	sprintf(_buf,"第%d级方块还有%d个,用时%d/%d秒",p->level(),p->count(),_time_current,_time_total);
	_box_text->text(_buf);
}
int box_info_struct::time(int t)
{
	_time_total = t;
}
int box_info_struct::sysevent(SDL_Event*e)
{
	//cout<<this<<endl;
	switch(e->type)
	{
		case SDL_USEREVENT:
			//cout<<"this:"<<this<<"msg is:"<<e->user.code<<endl;
			//cout<<e->user.code<<endl;
			switch(e->user.code)
			{
				case sdlgui_button_click:
					//cout<<this<<endl;
					parent()->event(e);
				break;
				case sdlgui_event_timer:
					_time_current++;
				break;
			}
		break;
	}
	return sdl_widget::sysevent(e);
}
#endif
