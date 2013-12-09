#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include <X11/Xlib.h>
#include <X11/X.h>
#include <X11/Xutil.h>
#include <X11/cursorfont.h>

#include <unistd.h>
#include <errno.h>

#include "error.h"
#include "utils.h"
#include "main.h"
#include "handlers.h"
#include "definitions.h"
#include "window.h"

int main(){	
	Display *display;
	Window root_window;
	Cursor cursor;
	display = XOpenDisplay(NULL);
	int cursor_shape = XC_arrow;
	ScreenInfos infos = get_screen_informations(display);
	if(display==NULL){
		printf("Cannot connect to display\n");
		return -1;
	}
	XSetErrorHandler(_X_error_handler);
	print_informations(display);
	infos = get_screen_informations(display);
	root_window= RootWindow(display, infos.screen_num);
	XStoreName(display, root_window, "Root Window");
	char root_color[] = "#1337AC";
	set_window_color(display, root_window, root_color);
	printf("Test Infos: %d\n", infos.width);
	cursor = XCreateFontCursor(display, cursor_shape);
	XDefineCursor(display, root_window, cursor);
	XSelectInput(display, root_window, ExposureMask | SubstructureNotifyMask |  ButtonPressMask | KeyPressMask | Button2MotionMask);
	//set_window_background(display,&gc, "background.png", root_window);
	printf("Background\n");
	XEvent local_event;
	XFlush(display);
	XGCValues values;	
	put_text(display, root_window,"Welcome to the uselesss window manager :)", "9x15", 50,50,BlackPixel(display,infos.screen_num), WhitePixel(display, infos.screen_num));
	Window cur_win;
	while(1){
		XNextEvent(display, &local_event);
		switch(local_event.type){
			case Expose:
				printf("Expose window event");
			break;
			case ConfigureNotify:
				configure_notify_handler(local_event, display);
			break;
			case MotionNotify:
				motion_handler(local_event, display);
			break;
			case CreateNotify:
				cur_win = local_event.xcreatewindow.window;
				char *window_name;
				XFetchName(display, cur_win, &window_name);
				printf("Window name: %s\n", window_name);
				if(window_name!=NULL){
					if(!strcmp(window_name, "Parent")){
						printf("Adding borders\n");
						XSetWindowBorderWidth(display, cur_win, BORDER_WIDTH);
					}
					XFree(window_name);
				}
			break;
			case MapNotify:
				printf("Map Notify\n");
				XWindowAttributes win_attr;
				char *child_name;
				XGetWindowAttributes(display, cur_win, &win_attr);
				XFetchName(display, local_event.xmap.window, &child_name);
				printf("Attributes: W: %d - H: %d - Name: %s\n", win_attr.width, win_attr.height, child_name);
				if(child_name!=NULL){
					if(strcmp(child_name, "Parent")){
						Window new_win = draw_window_with_name(display,root_window, "Parent", infos.screen_num, win_attr.x, win_attr.y, win_attr.width, win_attr.height+DECORATION_HEIGHT, 0, BlackPixel(display, infos.screen_num));				  
						XMapWindow(display, new_win);
						XReparentWindow(display,local_event.xmap.window, new_win,0, DECORATION_HEIGHT);
						put_text(display, new_win, child_name, "9x15", 10, 10, BlackPixel(display,infos.screen_num), WhitePixel(display, infos.screen_num));
					}
					XFree(child_name);
				}
			break;
			case ButtonPress:
				printf("Event button pressed\n");
				button_handler(local_event, display, infos);
				//Window launcher_win = create_launcher(display, root_window,infos);
			break;
			case KeyPress:
				printf("Keyboard key pressed\n");
				keyboard_handler(local_event, display);
			break;
			default: 
				printf("default event %d\n", local_event.type);
			break;
		}
	}
	XCloseDisplay(display);
}
