#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include <unistd.h>
#include <signal.h>
#include <errno.h>
#include <X11/Xlib.h>
#include <X11/X.h>
#include <X11/Xutil.h>

#include "definitions.h"
#include "handlers.h"
#include "window.h"
#include "main.h"
#include "manager.h"

void button_handler(XEvent event,Display *display, ScreenInfos infos){
  switch(event.xbutton.button){
    case Button1:
      printf("---Left Button pressed---\n");
      unsigned long _pid = get_window_pid(display, event.xbutton.window);
      printf("Check return value: %lu\n", _pid);
      break;
    case Button2:
      printf("---Mid Button pressed---\n");
      if(event.xbutton.button==Button2){
		Window cur_win = draw_window_with_name(display, event.xbutton.root, "TestWindow",infos.screen_num, 350,350, 200,200,BORDER_NONE, WhitePixel(display, infos.screen_num));
		XMapWindow(display, cur_win);
	}
      break;
    case Button3:
      printf("---Right Button pressed---\n");      
      Window current_window = event.xbutton.subwindow;
      char *window_name;
      if(current_window!=None){
	XFetchName(event.xbutton.display, current_window, &window_name);
	printf("Window name: %s - ", window_name);
	unsigned long _pid_kill = get_window_pid(display, event.xbutton.subwindow);
	XDestroyWindow(event.xbutton.display, current_window);
	printf("To kill: %d -", _pid_kill);
	if(_pid_kill!=0){
	  kill(_pid_kill, SIGKILL);
	}
	printf("Current_Window != none");
      } else {
	//XFetchName(event.xbutton.display, event.xbutton.window, &window_name);
	printf("Current_Window == none");
      }
      printf(" Click on: %s\n", window_name);
      XFree(window_name);
      break;
  }
  printf("---End---\n");
}

char keyboard_handler(XEvent event, Display* display){
  int keysyms_per_keycode_return;
  KeySym key_symbol = XGetKeyboardMapping(display,event.xkey.keycode,1, &keysyms_per_keycode_return);
  if(XLookupKeysym(&event.xkey, 0)== XK_1){
    printf("ci sono\n");
    int pid = fork();
    if(pid==0) {      
	if(execlp(DEFAULT_TERMINAL, DEFAULT_TERMINAL, "-ut", 0)==-1){
	  printf("%s", strerror(errno));
	}
    }
  }
  int ascii_key = key_symbol - XK_A + 'A';
}

void motion_handler(XEvent event, Display *display){
    printf("Motion notify event\n");
    Window cur_window = event.xmotion.window;
    int x = event.xmotion.x_root;
    int y = event.xmotion.y_root;
    //XMoveWindow(display, cur_window, x,y);
}

void configure_notify_handler(XEvent local_event, Display* display){
	printf("Configure notify Event\n");
	Window cur_win = local_event.xconfigurerequest.window;
	XWindowAttributes win_attr;
}

void map_notify_handler(XEvent local_event, Display* display, ScreenInfos infos){
	printf("Map Notify\n");
	XWindowAttributes win_attr;
	char *child_name;
	XGetWindowAttributes(display, local_event.xmap.window, &win_attr);
	XFetchName(display, local_event.xmap.window, &child_name);
	printf("Attributes: W: %d - H: %d - Name: %s - ID %lu\n", win_attr.width, win_attr.height, child_name, local_event.xmap.window);
	if(child_name!=NULL){
	  if(strcmp(child_name, "Parent") && local_event.xmap.override_redirect == False){
		Window new_win = draw_window_with_name(display, RootWindow(display, infos.screen_num), "Parent", infos.screen_num, 
						   win_attr.x, win_attr.y, win_attr.width, win_attr.height+DECORATION_HEIGHT, 0, 
						   BlackPixel(display, infos.screen_num));
		XMapWindow(display, new_win);
		XReparentWindow(display,local_event.xmap.window, new_win,0, DECORATION_HEIGHT);
		set_window_item(local_event.xmap.window, new_win);
		XSelectInput(display, local_event.xmap.window, StructureNotifyMask);
		printf("Parent window id: %lu\n", new_win);
		put_text(display, new_win, child_name, "9x15", 10, 10, BlackPixel(display,infos.screen_num), WhitePixel(display, infos.screen_num));
	  }
	}
	XFree(child_name);
}

void destroy_notify_handler(XEvent local_event, Display *display){			
	Window window = local_event.xdestroywindow.event;
	printf("Window to destroy: %lu\n", local_event.xdestroywindow.window);
	WindowItem item = get_window_item(local_event.xdestroywindow.window);
	if(item.key!=0){
		printf("Destroying Window %lu - key: %lu\n", item.value,item.key);
		//XDestroyWindow(display, item.value);
		destroy_window_item(item.key);
	}
	
}
