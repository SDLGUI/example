#include "iostream"
#include <stdlib.h>
#include <stdio.h>
#include <conio.h>
#include "sdlgui/sdlgui.h"
#include "core/hitbox_data.cpp"
#include "game_gui/plane_gui.cpp"
int sw,sh;
using namespace std;
//hitbox_plane_struct plane;
plane_gui_struct_ptr view = NULL;
sdl_frame frame("hit Box",100,100,300,330,SDL_WINDOW_SHOWN);
int frame_event(sdl_frame_ptr obj,SDL_Event *e)
{
	switch(e->type)
	{
		case SDL_KEYUP:
			if(view)
			{
				/* 让方块向指定方块移动 */
				switch(e->key.keysym.sym)
				{
					case SDLK_UP:
						view->move(box_move_up);
					break;
					case SDLK_DOWN:
						view->move(box_move_down);
					break;
					case SDLK_LEFT:
						view->move(box_move_left);
					break;
					case SDLK_RIGHT:
						view->move(box_move_right);
					break;
				}
				view->info();
			}
		break;
	}
}
int main(int argc,char** argv)
{
	view = frame.add<plane_gui_struct>("",0,0,300,330,1);
	frame.ime.hide();
	frame.event(frame_event);
	return frame.run();
}
