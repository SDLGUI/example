#include <cmath>
#include "../sdlgui/sdlgui.h"
#include "../core/hitbox_data.cpp"
using namespace std;
//-------------------------------------------------------------------------------
/* 
		用于处理活动方块显示效果 
 */
typedef class hitbox_floatbox_struct : public GUI<hitbox_floatbox_struct,sdl_widget>
{
	public:
		hitbox_floatbox_struct();
		int init();
		int init(const char*,int,int,int,int,Uint32);
		int sysevent(SDL_Event*);
	public:
		int push(int,int);
		int pull();
		Uint32 _alpha;
		int _alpha_r;
}*hitbox_floatbox_struct_ptr;
hitbox_floatbox_struct::hitbox_floatbox_struct()
:
GUI<hitbox_floatbox_struct,sdl_widget>()
{
	init();
}
int hitbox_floatbox_struct::init()
{
	_alpha = 100;
	_alpha_r = 1;
	return 0;
}
int hitbox_floatbox_struct::init(const char* ptitle,int px,int py,int pw,int ph,Uint32 pflags)
{
	if(sdl_widget::init(ptitle,px,py,pw,ph,pflags))return -1;
	blend(SDL_BLENDMODE_BLEND);
	alpha(_alpha);
	add_timer(100);
	return 0;
}
int hitbox_floatbox_struct::sysevent(SDL_Event* e)
{
	switch(e->type)
	{
		case SDL_USEREVENT:
			_alpha += (_alpha/5)*_alpha_r;
			if(_alpha>=255)
			{
				_alpha = 255;
				_alpha_r = -1;
			}
			else
			if(_alpha<100)
			{
				_alpha = 100;
				_alpha_r = 1;
			}
			alpha(_alpha);
		break;
	}
}
//---------------------------------------------------------------------------
/* 
	用于显示填充方块显示效果 
 */
//---------------------------------------------------------------------------
typedef class hitbox_pushbox_struct : public GUI<hitbox_pushbox_struct,sdl_widget>
{
	public:
		hitbox_pushbox_struct();
		int init();
		int init(const char*,int,int,int,int,Uint32);
		int sysevent(SDL_Event*);
	public:
		int push(int,int,int,int);
		int pull(int,int,int,int);
	protected:
		Uint32 _alpha;
		int _alpha_r;
		int _clip_size;
		SDL_Rect hit_pos;
		int _status;
		SDL_Point offset;
}*hitbox_pushbox_struct_ptr;
hitbox_pushbox_struct::hitbox_pushbox_struct()
:
GUI<hitbox_pushbox_struct,sdl_widget>()
{
	init();
}
int hitbox_pushbox_struct::init()
{
	if(sdl_widget::init())return -1;
	return 0;
}
int hitbox_pushbox_struct::init(const char* ptitle,int px,int py,int pw,int ph,Uint32 pflags)
{
	if(sdl_widget::init(ptitle,px,py,pw,ph,pflags))return -1;
	_status = 0;
	_clip_size = pw;
	_alpha_r = 1;
	fill_rect(NULL,0x00ff00);
	blend(SDL_BLENDMODE_BLEND);
	alpha(150);
	add_timer(100);
	return 0;
}
int hitbox_pushbox_struct::push(int fx,int fy,int x,int y)
{
	hit_pos.x = fx*_clip_size;
	hit_pos.y = fy*_clip_size;
	hit_pos.w = x*_clip_size;
	hit_pos.h = y*_clip_size;
	_status = 1;
	offset.x = (hit_pos.w-hit_pos.x)/5;
	offset.y = (hit_pos.h-hit_pos.y)/5;
	return pos(hit_pos.x,hit_pos.y);
}
int hitbox_pushbox_struct::pull(int fx,int fy,int x,int y)
{
	hit_pos.x = fx*_clip_size;
	hit_pos.y = fy*_clip_size;
	hit_pos.w = x*_clip_size;
	hit_pos.h = y*_clip_size;
	_status = 2;
	offset.x = (hit_pos.w-hit_pos.x)/5;
	offset.y = (hit_pos.h-hit_pos.y)/5;
	return 0;
	//return pos(600,600);
}
int hitbox_pushbox_struct::sysevent(SDL_Event * e)
{
	switch(e->type)
	{
		case SDL_USEREVENT:
			switch(e->user.code)
			{
				case sdlgui_event_timer:
					switch(_status)
					{
						/* 推送方块 */
						case 1:
							cout<<"push"<<endl;
							if(abs(hit_pos.w-hit_pos.x)>10 || abs(hit_pos.h-hit_pos.y)>10)
							{
								//cout<<hit_pos.x<<":"<<hit_pos.y<<endl;
								hit_pos.x+=offset.x;
								hit_pos.y+=offset.y;
								pos(hit_pos.x,hit_pos.y);
							}
							else
							{
								pos(hit_pos.w,hit_pos.h);
								_status = 0;
							}
						break;
						/* 摘取方块 */
						case 2:
							cout<<"pull"<<endl;
							if(abs(hit_pos.w-hit_pos.x)>10 || abs(hit_pos.h-hit_pos.y)>10)
							{
								hit_pos.w-=offset.x;
								hit_pos.h-=offset.y;
								pos(hit_pos.w,hit_pos.h);
							}
							else
							{
								pos(600,600);
								_status = 0;
							}
						break;
					}
					//cout<<this<<endl;
				break;
			}
		break;
	}
	return sdl_widget::sysevent(e);
}
//----------------------------------------------------------------------------
/*
		用于显示和接收游戏行为
 */
typedef class plane_gui_struct : public GUI<plane_gui_struct,sdl_widget>
{
	public:
		plane_gui_struct();
		int init();
		int init(const char*,int,int,int,int,Uint32);
		int sysevent(SDL_Event*);
	public:
		/* 更新盘面 */
		int update();
		/* 开始游戏 */
		int start(int);
		/* 更新信息 */
		int info();
		/* 移动方块 */
		int move(int);
	protected:
		hitbox_pushbox_struct_ptr set_array(int);
	protected:
		/* 盘面背景 */
		sdl_clip hitbox_bg;
		/* 剪辑大小 */
		int _clip_size;
		/* 游戏数据 */
		hitbox_plane_struct plane_data;
		/* 信息栏 */
		sdl_widget_ptr _info;
		/* 开始按钮 */
		sdl_button_ptr _start;
		/* 游戏过关界面 */
		sdl_widget_ptr _win;
		/* 活动方块 */
		hitbox_floatbox_struct_ptr _float_box;
		/* 填充方块,默认为3个 */
		hitbox_pushbox_struct_ptr _push_box[4];
		hitbox_pushbox_struct_ptr _t[4];
		int pid[4];
}*plane_gui_struct_ptr;
plane_gui_struct::plane_gui_struct()
:
GUI<plane_gui_struct,sdl_widget>()
{

}
int plane_gui_struct::init()
{

}
int plane_gui_struct::init(const char* ptitle,int px,int py,int pw,int ph,Uint32 pflag)
{
	int i;
	if(sdl_widget::init(ptitle,px,py,pw,ph,pflag))return -1;
	_clip_size = pw/box_width;
	/* 加入背景 */
	hitbox_bg.img_load("bg.jpg");
	/* 生成背景剪辑 */
	hitbox_bg.clip(_clip_size,_clip_size);
	/* 更新背景状态 */
	update();
	/* 添加状态栏 */
	_info = add<sdl_widget>("",0,ph-30,pw-40,30,1);
	/* 添加开始按钮 */
	_start = add<sdl_button>("开始",pw-40,ph-30,40,30,1);
	blit_surface(_info->rect(),_info,NULL);
	/* 更新状态栏 */
	info();
	/* 添加活动方块 */
	_float_box = add<hitbox_floatbox_struct>("",0,0,_clip_size,_clip_size,1);
	_float_box->fill_rect(NULL,0x00ff00);
	/* 添加填充方块 */
	for(i=0;i<3;i++)
	{
		_push_box[i] = add<hitbox_pushbox_struct>("",600,600,_clip_size,_clip_size,1);
		_t[i] = _push_box[i];
		pid[i]=0;
	}
	_win = add<sdl_widget>("",0,0,300,300,1);
	hitbox_bg.blit_surface(NULL,_win,NULL);
	_win->hide();
	return 0;
}
int plane_gui_struct::update()
{
	int x,y;
	SDL_Rect rt;
	hitbox_bg.blit_surface(NULL,this,NULL);
	rt.x = 0;
	rt.y = 0;
	rt.w = _clip_size*box_width;
	rt.h = _clip_size*box_width;
	fill_rect(&rt,0x000000);
	rt.w = _clip_size;
	rt.h = _clip_size;
	for(y=0;y<box_height;y++)
	{
		for(x=0;x<box_width;x++)
		{
			hitbox_bg(x,y)->surface_blend_mode(SDL_BLENDMODE_BLEND);
			rt.x = x*_clip_size;
			rt.y = y*_clip_size;
			/* 按方块状态更新背景 */
			switch(plane_data.box_id(x,y))
			{
				case bind_box_id:
					hitbox_bg(x,y)->surface_alpha_mod(255);
				break;
				case push_box_id:
					hitbox_bg(x,y)->surface_alpha_mod(55);
				break;
				case empty_box_id:
					hitbox_bg(x,y)->surface_alpha_mod(150);
				break;
				case pass_box_id:
					hitbox_bg(x,y)->surface_alpha_mod(100);
				break;
			}
			/* 将背景刷新到窗口 */
			hitbox_bg(x,y)->blit_surface(NULL,this,&rt);
		}
	}
	return 0;
}
int plane_gui_struct::start(int plevel)
{
	int i;
	plane_data.level((!plane_data.check())?plevel:plane_data.level());
	update();
	for(i=0;i<3;i++)
	{
		_push_box[i]->pos(600,600);
		_t[i] = _push_box[i];
		pid[i]=0;
	}
	info();
	if(_win)_win->hide();
	return 0;
}
int plane_gui_struct::info()
{
	char buf[1000];
	memset(buf,0x00,1000);
	sprintf(buf,"你在第%d级还有%d个方块可以使用",plane_data.level(),plane_data.current_count());
	if(_info)_info->text(buf);
}
int plane_gui_struct::move(int r)
{
	hitbox_pushbox_struct_ptr tbox;	
	int d = plane_data.move(r); 
	int x,y;
	/* 移动方块出错 */
	if(d == -1)
	{
		cout<<"Move Error"<<endl;
		return -1;
	}
	else
	/* 成功 */
	if(d == 0)
	{
		cout<<"win!!"<<endl;
		if(_win)_win->show();
		return 0;
	}
	else
	/* 移动方块 */
	if(d<100)
	{
		d = d-10;
		x = d/10;
		y = d-x*10;
		_float_box->pos(x*_clip_size,y*_clip_size);
	}
	else
	/* 摘取方块 */
	if(d<1000)
	{
		cout<<"pull:"<<d<<endl;
		tbox = set_array(d);
		if(tbox)
		{
			d = d-100;
			x = d/10;
			y = d - x*10;
			tbox->pull(plane_data._float_box_x,plane_data._float_box_y,x,y);
			//tbox->pos(600,600);
		}
	}
	else
	/* 填充方块 */
	{
		tbox = set_array(d);
		cout<<"push:"<<d<<endl;
		if(tbox)
		{
			d = d-1000;
			x = d/10;
			y = d-x*10;
			tbox->push(plane_data._float_box_x,plane_data._float_box_y,x,y);
			//tbox->pos(x*_clip_size,y*_clip_size);
		}
	}
	return 0;
}
/* 
	当ID为0时表示重设ID
	如果ID不存在表示填充方块，否则表示摘取方块 
	填充时把未ID放到头ID
	摘取时把指定ID放到未ID
 */
hitbox_pushbox_struct_ptr plane_gui_struct::set_array(int id)
{
	int i,n;
	int tid = (id<1000)?id-100:id-1000;
	if(tid)
	{
		for(i=0;i<3;i++)
		{
			/* 如果这个ID存在表示摘取 */
			if(pid[i] == tid)			
			{
				/*
					摘取时先把指定ID放到最后一个数据中临时存放
					再把指定数据后面的数据向前移动一格 
					然后清除第3个数据ID	
				 */
				_t[3] = _t[i];
				for(n=i;n<3;n++)
				{
					_t[n] = _t[n+1];
					pid[n] = pid[n+1];
				}
				pid[2] = 0;
				return _t[2];
			}
		}
		/* 
			如果没有返回表示填充方块
			先把第1个之后的方块向后移动一格，空出第一个数据用来存放填充的方块 
			同时更新方块ID
			将末尾要填充的方块存放到第一格并更新第一个方块ID
		 */
		for(i=3;i;i--)
		{
			_t[i]=_t[i-1];
			pid[i] = pid[i-1];
		}
		_t[0] = _t[3];
		pid[0] = tid;
		return _t[0];
	}
	else
	{
		/* 初始化填充存储空间 */
		memcpy(_t,_push_box,sizeof(sdl_frame_ptr)*4);
		memset((char*)&pid,0x00,sizeof(pid));
		return NULL;
	}
}
int plane_gui_struct::sysevent(SDL_Event* e)
{
	switch(e->type)
	{
		case SDL_USEREVENT:
			start(plane_data.level()+1);
			active();
		break;
	}
	return sdl_widget::sysevent(e);
}
