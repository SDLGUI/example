#ifndef __hitbox_data__
#define __hitbox_data__
#include "iostream"
#include <cmath>
using namespace std;
/* 固定的方块ID */
const int bind_box_id = 8;
/* 指定填充的方块ID */
const int push_box_id = 3;
/* 空方块ID */
const int empty_box_id = 2;
/* 要行走方块ID */
const int	pass_box_id = 7; 
/* 追加方块时的调整值 */
const int add_box_id = 2;

const int box_width = 7;
const int box_height= 7;

const int level_max = 7;

const int box_move_up = 1;
const int box_move_down = 2;
const int box_move_left = 3;
const int box_move_right = 4;

const double PI = 3.1415926;


struct hitbox_struct
{
	int _id;
	hitbox_struct();
};
hitbox_struct::hitbox_struct()
{
	_id = empty_box_id;
}

struct hitbox_level_bind_struct
{
	int _a_bind_id[2];
	int _b_bind_id[2];
	int _a_id[2];
	int _b_id[2];
	int _c_id[2];
	int _box_count;
	int init(int,int,int,int,int,int,int,int,int,int,int);
};
int hitbox_level_bind_struct::init(int pcount,
																	 int bax,int bay,
																	 int bbx,int bby,
																	 int ax,int ay,
																	 int bx,int by,
																	 int cx,int cy)
{
	_a_bind_id[0] = bax;
	_a_bind_id[1] = bay;
	//
	_b_bind_id[0] = bbx;
	_b_bind_id[1] = bby;
	//
	_a_id[0] = ax;
	_a_id[1] = ay;
	//
	_b_id[0] = bx;
	_b_id[1] = by;
	//
	_c_id[0] = cx;
	_c_id[1] = cy;
	//
	_box_count = pcount;
	return 0;
}

struct hitbox_plane_struct
{
	hitbox_struct hitbox[box_width][box_height];
	/* 游戏层级设置方块 */
	hitbox_level_bind_struct _level_bind[level_max];
	/* 游戏层级 */
	int _level;
	/* 方块个数 */
	int _box_count;
	/* 活动方块的坐标 */
	int _float_box_x;
	int _float_box_y;
	/* 构造函数 */
	hitbox_plane_struct();
	/* 方块个数 */
	int count();
	/* 游戏级别 */
	int level(int);
	int level();
	/* 游戏检测 */
	int check();
	/* 方块推送 */
	int push(int,int);
	/* 方块拉取 */
	int pull(int,int);
	/* 方块移动 */
	int move(int);
	/* 方块方位 */
	int angle();
	/* 方块ID */
	int box_id(int,int);
	int a_bind_pos();
	int b_bind_pos();
	int a_push_pos();
	int b_push_pos();
	int c_push_pos();
};
hitbox_plane_struct::hitbox_plane_struct()
{
	/* 第一个参数为方块个数，
		第二，三个参数为第1个固定点的方块坐标， 
		第四，五个参数为第2个固定点的方块坐标， 
		第六，七个参数为第1个填充点的方块坐标， 
		第八，九个参数为第2个填充点的方块坐标， 
		第十，十一个参数为第3个填充点的方块坐标， 
	 */
	_level_bind[0].init(3,4,4,5,4,3,3,2,4,3,6);
	_level_bind[1].init(3,4,4,5,4,3,3,2,4,3,6);
	_level_bind[2].init(3,4,4,5,4,3,3,2,4,3,6);
	_level_bind[3].init(3,4,4,5,4,3,3,2,4,3,6);
	_level_bind[4].init(3,4,4,5,4,3,3,2,4,3,6);
	_level_bind[5].init(3,4,4,5,4,3,3,2,4,3,6);
	_level_bind[6].init(3,4,4,5,4,3,3,2,4,3,6);
	level(0);
	_float_box_x = 0;
	_float_box_y = 0;
}
int hitbox_plane_struct::level()
{
	return _level;
}
int hitbox_plane_struct::level(int pl)
{
	_level = (pl > level_max-1)?level_max-1 : pl;
	/* 设置游戏状态 */	
	_box_count = _level_bind[_level]._box_count;
	int x;
	int y;
	//
	x = _level_bind[_level]._a_bind_id[0];
	y = _level_bind[_level]._a_bind_id[1];
	hitbox[x][y]._id = bind_box_id;
	//
	x = _level_bind[_level]._b_bind_id[0];
	y = _level_bind[_level]._b_bind_id[1];
	hitbox[x][y]._id = bind_box_id;
	//
	x = _level_bind[_level]._a_id[0];
	y = _level_bind[_level]._a_id[1];
	hitbox[x][y]._id = push_box_id;
	//
	x = _level_bind[_level]._b_id[0];
	y = _level_bind[_level]._b_id[1];
	hitbox[x][y]._id = push_box_id;
	//
	x = _level_bind[_level]._c_id[0];
	y = _level_bind[_level]._c_id[1];
	hitbox[x][y]._id = push_box_id;
	//
	for(x=0;x<box_width;x++)
	{
		hitbox[x][0]._id = pass_box_id;
		hitbox[x][box_height-1]._id = pass_box_id;
	}
	for(x=1;x<box_height-1;x++)
	{
		hitbox[0][x]._id = pass_box_id;
		hitbox[box_width-1][x]._id = pass_box_id;
	}
	return 0;
}
int hitbox_plane_struct::push(int px,int py)
{
	if(hitbox[px][py]._id < pass_box_id)	
	{
		hitbox[px][py]._id += add_box_id;
		_box_count --;
		return 0;
	}
	return -1;
}
int hitbox_plane_struct::pull(int px,int py)
{
	if(hitbox[px][py]._id>push_box_id)
	{
		hitbox[px][py]._id -= add_box_id;
		_box_count ++;
		return 0;
	}
	return -1;
}
int hitbox_plane_struct::move(int r)
{
	int tx,ty,_state;
	_state = 0;
	/* 先检测向指定方向移动,确定是移动还是填充或摘取 */
	/* 如果_state为1表示越界，可能是摘取 */
	switch(r)
	{
		case box_move_up:
			if(!_float_box_y) _state = 1;
		break;
		case box_move_down:
			if(_float_box_y>=(box_height-1))_state = 1;
		break;
		case box_move_left:
			if(!_float_box_x)_state = 1;
		break;
		case box_move_right:
			if(_float_box_x>=(box_width-1))_state = 1;
		break;
	}
	tx = _float_box_x;
	ty = _float_box_y;
	switch(_state)
	{
		/* 在范围内 */
		case 0:
			/* 如果是移动活动方块 */
			switch(r)
			{
				case box_move_up:
					if(hitbox[tx][ty-1]._id==pass_box_id)
					{
						_float_box_y-=1;
						return box_move_up;
					}
				break;
				case box_move_down:
					if(hitbox[tx][ty+1]._id==pass_box_id)
					{
						_float_box_y+=1;
						return box_move_down;
					}
				break;
				case box_move_left:
					if(hitbox[tx-1][ty]._id==pass_box_id)
					{
						_float_box_x-=1;
						return box_move_left;
					}
				break;
				case box_move_right:
					if(hitbox[tx+1][ty]._id==pass_box_id)
					{
						_float_box_x+=1;
						return box_move_right;
					}
				break;
			}
			/* 如果不是移动活动方块，就是填充方块 */
			/* 如果没有可以填充用的方块，则返回错误 */
			if(!_box_count)return -1;
			switch(r)
			{
				case box_move_up:
					ty-=1;
				break;
				case box_move_down:
					ty+=1;
				break;
				case box_move_left:
					tx-=1;
				break;
				case box_move_right:
					tx+=1;
				break;
			}
			/* 检测是否可以填充方块 */
			while((tx) && (tx<box_width) && (ty) && (ty<box_height))
			{
				/* 如果可以填充方块，返回被检测方块的引索(矢量转为整形)
					并设置方块状态 
				 */
				if(hitbox[tx][ty]._id>push_box_id && hitbox[tx][ty]._id != pass_box_id)
				{
					switch(r)
					{
						case box_move_up:
							if(hitbox[tx][ty+1]._id < pass_box_id)
							{
								push(tx,ty+1);
								return (tx*10+(ty+1))+1000;					
							}
						break;
						case box_move_down:
							if(hitbox[tx][ty-1]._id < pass_box_id)
							{
								push(tx,ty-1);
								return (tx*10+(ty-1))+1000;					
							}
						break;
						case box_move_left:
							if(hitbox[tx+1][ty]._id < pass_box_id)
							{
								push(tx+1,ty);
								return ((tx+1)*10+ty)+1000;					
							}
						break;
						case box_move_right:
							if(hitbox[tx-1][ty]._id < pass_box_id)
							{
								push(tx-1,ty);
								return ((tx-1)*10+ty)+1000;					
							}
						break;
					}
				}
				/* 用于循环检测坐标 */
				switch(r)
				{
					case box_move_up:
						ty--;
					break;
					case box_move_down:
						ty++;
					break;
					case box_move_left:
						tx--;
					break;
					case box_move_right:
						tx++;
					break;
				}
			}
			/* 如果没有返回表示不能填充，返回错误 */
			return -1;
		break;
		/* _state==1检测是否摘取,还是越界 */
		case 1:
			while((tx>-1) && (tx<box_width) && (ty>-1) && (ty<box_height))
			{
				/* 如果被检测的方块ID比填充方块ID大，并且比行走方块ID小
					返回被检测方块的引索（矢量转为整形) 
				 */
				if((hitbox[tx][ty]._id>(push_box_id)) && (hitbox[tx][ty]._id<pass_box_id))
				{
					pull(tx,ty);
					return tx*10+ty+100;	
				}
				/* 用于循环检测坐标 */
				switch(r)
				{
					case box_move_up: ty++; break;
					case box_move_down: ty--; break;
					case box_move_left: tx++; break;
					case box_move_right: tx--; break;
				}
			}
			/* 如果没有返回表示越界了，则返回错误 */
			return -1;
		break;
	}
}
int hitbox_plane_struct::check()
{
	int x,y;
	for(x=0;x<box_width;x++)
	{
		for(y=0;y<box_height;y++)
		{
			if(box_id(y,x) == push_box_id)return -1;
		}
	}
	return 0;
}
int hitbox_plane_struct::angle()
{
	int x,y,r;
	x = (_float_box_x - box_width/2);
	y = (_float_box_y - box_height/2);
	r = atan(abs(y)/abs(x))/PI*180;
	return r;
}
int hitbox_plane_struct::box_id(int px,int py)
{
	return hitbox[px][py]._id;
}
int hitbox_plane_struct::a_bind_pos()
{
	int x,y;
	x = _level_bind[_level]._a_bind_id[0];
	y = _level_bind[_level]._a_bind_id[1];
	return x*10+y;
}
int hitbox_plane_struct::b_bind_pos()
{
	int x,y;
	x = _level_bind[_level]._b_bind_id[0];
	y = _level_bind[_level]._b_bind_id[1];
	return x*10+y;
}
int hitbox_plane_struct::a_push_pos()
{
	int x,y;
	x = _level_bind[_level]._a_id[0];
	y = _level_bind[_level]._a_id[1];
	return x*10+y;
}
int hitbox_plane_struct::b_push_pos()
{
	int x,y;
	x = _level_bind[_level]._b_id[0];
	y = _level_bind[_level]._b_id[1];
	return x*10+y;
}
int hitbox_plane_struct::c_push_pos()
{
	int x,y;
	x = _level_bind[_level]._c_id[0];
	y = _level_bind[_level]._c_id[1];
	return x*10+y;
}
int hitbox_plane_struct::count()
{
	return _box_count;
}
#endif
