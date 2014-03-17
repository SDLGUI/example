#include "sdlgui.h"
using namespace std;
struct box_plane_item
{
	public:
		int _id;
	
};
class box_item : public GUI<box_item,sdl_widget>
{
	pubic:

		box_item(const char*,int,int,int,int,Uint32);
		int init(const char*,int,int,int,int,Uint32);
		int init();
		int id(int);
		int id();
		int pos();
	public:
		int _id;
}
box_item::box_item(const char* ptitle,int px,int py,int pw,int ph,Uint32 pflag)
:
sdl_widget(ptitle,px,py,pw,ph,pflag)
{
	init(ptitle,px,py,pw,ph,pflag);
}
int box_item::init()
{
	_id = 0;
	return 0;
}
int box_item::init(const char* ptitle,int px,int py,int pw,int ph,Uint32 pflag)
{
	init();
	if(sdl_widget::init(ptitle,px,py,pw,ph,pflag))return -1;
	return 0;
}
int box_item::id(int pid)
{
	_id = pid;
	return 0;
}
int box_item::id()
{
	return _id;
}
