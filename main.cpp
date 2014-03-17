#include "iostream"
#include <stdlib.h>
#include <stdio.h>
#include <conio.h>
#include "sdlgui.h"
#include "hitbox_data.cpp"
#include "plane_gui.cpp"
int sw,sh;
using namespace std;
//hitbox_plane_struct plane;
plane_gui_ptr view = NULL;
box_info_struct_ptr info = NULL;
sdl_widget_ptr w;
sdl_widget_ptr w1,w2,w3;
sdl_frame frame("hit Box",100,100,gw,300,SDL_WINDOW_SHOWN);
 
int frame_proc(sdl_frame_ptr obj,SDL_Event* e)
{
	int v;
	int x,y;
	switch(e->type)
	{
		case SDL_USEREVENT:
			switch(e->user.code)
			{
				case sdlgui_button_click:
					view->event(e);
					//cout<<1<<endl;
				break;
				case sdlgui_event_timer:
					if(info)info->info(&(view->plane));
				break;
			}
		break;
	}
	return 0;
}
int main(int argc,char** argv)
{
	info = frame.add<box_info_struct>("",0,gh,gw,30,1);
	view = frame.add<plane_gui>("",0,0,gw,gh,1);
	info->time(90);
	cout<<info<<endl;
	frame.add_timer(100);
	frame.event(frame_proc);
	frame.ime.hide();
	return frame.run();
}
