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
#include "background.h"
#include "manager.h"

int main(int argc, char **argv){	
	Display *display;
	Window root_window;
	Cursor cursor;
	display = XOpenDisplay(NULL);
	int cursor_shape = XC_arrow;
	char *filename;
	ScreenInfos infos = get_screen_informations(display);
	if(display==NULL){
		printf("Cannot connect to display\n");
		return -1;
	}
	XSetErrorHandler(_X_error_handler);
	print_informations(display);
	init_manager();
	infos = get_screen_informations(display);
	root_window= RootWindow(display, infos.screen_num);
	XStoreName(display, root_window, "Root Window");
	cursor = XCreateFontCursor(display, cursor_shape);
	XDefineCursor(display, root_window, cursor);
	XSelectInput(display, root_window, EnterWindowMask | ExposureMask | SubstructureNotifyMask | ButtonPressMask | KeyPressMask | Button2MotionMask | PropertyChangeMask);
	GC gc;
	XEvent local_event;
	XFlush(display);
	XGCValues values;
	//TODO: Improve argument parser
	if(argc>=2){
		set_window_background(display,&gc, argv[1], root_window);
	} else {
		set_window_color(display, root_window, DEFAULT_COLOR);
	}
	put_text(display, root_window,"Welcome to the uselesss window manager :) :)", "9x15", 50,50,BlackPixel(display,infos.screen_num), WhitePixel(display, infos.screen_num));
	Window cur_win;
	while(1){
		XNextEvent(display, &local_event);
		switch(local_event.type){
			case EnterNotify:
				printf("Enter Notify event");
			break;
			case Expose:
				printf("Expose window event %lu\n", local_event.xexpose.window);
			break;
			case ConfigureNotify:
				configure_notify_handler(local_event, display);
			break;
			case MapRequest:
				printf("----Map Request!-----\n");
			break;
			case PropertyNotify:
				printf("Property notify event: %d\n", local_event.xproperty.state);
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
			case ReparentNotify:
				if(local_event.xreparent.override_redirect == True) printf("Override True\n");
				else printf("Override False\n");
			break;
			case MapNotify:
				map_notify_handler(local_event,display, infos);
			break;
			case UnmapNotify: 
				printf("UnMapNotify\n");
			break;
			case DestroyNotify:
				printf("Destroy Event\n");
				destroy_notify_handler(local_event,display);
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
			case ClientMessage:
				client_message_handler(local_event, display);
				//Atom wm_state = XInternAtom(display, XGetAtomName(display,local_event.xclient.message_type), True);
				//unsigned long nitems_return;
				//unsigned char *prop_return;
				//get_property_value(display, local_event.xclient.window, XGetAtomName(display,local_event.xclient.message_type), 256, &nitems_return, (unsigned char **)&prop_return);
			break;
			case MappingNotify:
				printf("Mapping notify Even\nt");
			break;
			default: 
				printf("default event %d\n", local_event.type);
			break;
		}
	}
	XCloseDisplay(display);
}

