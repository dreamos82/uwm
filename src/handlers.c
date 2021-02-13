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
#include "utils.h"
#include "launcher.h"
#include "icons.h"

void button_handler(XEvent event,Display *display, ScreenInfos infos){
  switch(event.xbutton.button){
    case Button1:
      printf("---Left Button pressed---\n");
      unsigned long _pid = get_window_pid(display, event.xbutton.window);
	  char *child_name;
	  XFetchName(display, event.xbutton.subwindow, &child_name);
	  printf("Window name: %s\n", child_name);
      printf("Check return value: %lu\n", _pid);
      printf("Coordinates: x=%d - y=%d\n", event.xbutton.x, event.xbutton.y);
      break;
    case Button2:
      printf("---Mid Button pressed---\n");
      if(event.xbutton.button==Button2){
		Window cur_win = draw_window_with_name(display, event.xbutton.root, "TestWindow",infos.screen_num, 350,350, 200,20,BORDER_NONE, WhitePixel(display, infos.screen_num));
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
	printf("To kill: %ld -", _pid_kill);
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

char keyboard_handler(XEvent event, Display* display, ScreenInfos infos){
  int keysyms_per_keycode_return;
  KeySym *key_symbol = XGetKeyboardMapping(display,event.xkey.keycode,1, &keysyms_per_keycode_return);
  if(XLookupKeysym(&event.xkey, 0)== XK_1){
    int pid = fork();
    if(pid==0) {      
		if(execlp(DEFAULT_TERMINAL, DEFAULT_TERMINAL, "-ut", 0)==-1){
		  printf("%s", strerror(errno));
		}
    }
  }
  if(XLookupKeysym(&event.xkey, 0)== XK_2){
	create_launcher(display, event.xkey.root, infos);
  }

  int ascii_key = *key_symbol - XK_A + 'A';
}

void expose_handler(XEvent event, Display *display){
    printf("----EXPOSE EVENT----\n");
    printf("\tsend_event: %d", event.xexpose.send_event);
    printf("\tcoords: x: %d, y: %d\n", event.xexpose.x, event.xexpose.y); 
    printf("\tsize: w: %d, h: %d\n", event.xexpose.width, event.xexpose.height);
    printf("--------------------\n");
    Window window = event.xexpose.window;
}

void motion_handler(XEvent event, Display *display){
    printf("Motion notify event\n");
    Window cur_window = event.xmotion.window;
    int x = event.xmotion.x_root;
    int y = event.xmotion.y_root;
    //XMoveWindow(display, cur_window, x,y);
}

void configure_notify_handler(XEvent local_event, Display* display){
	//printf("Configure notify Event\n");
	Window cur_win = local_event.xconfigurerequest.window;
	XWindowAttributes win_attr;
	int event_type = local_event.xconfigurerequest.type;
	printf("----CONFIGURE_NOTIFY_EVENT----\n");
	printf("\tType: %d\n", event_type);
	if(local_event.xconfigurerequest.above == None){
		printf("\tWindow is none\n");
	} else {
		printf("\tWindow is not None\n");
	}
	printf("----------------\n");
	//XMapWindow(local_event.xconfigurerequest.display, cur_win);
	//XCirculateSubwindowsUp(local_event.xconfigurerequest.display, cur_win);
	//XRaiseWindow(local_event.xconfigurerequest.display, cur_win);
}

void map_notify_handler(XEvent local_event, Display* display, ScreenInfos infos){
	printf("----------Map Notify\n");
	XWindowAttributes win_attr;
	char *child_name;
	XGetWindowAttributes(display, local_event.xmap.window, &win_attr);
	XFetchName(display, local_event.xmap.window, &child_name);
	printf("\tAttributes: W: %d - H: %d - Name: %s - ID %lu\n", win_attr.width, win_attr.height, child_name, local_event.xmap.window);
	Window trans = None;	
	XGetTransientForHint(display, local_event.xmap.window, &trans);	
	printf("\tIs transient: %ld\n", trans);
	if(trans != None){
			printf("\t\tIs Trans\n");
			XWMHints *hints = XGetWMHints(display, trans);
			printf("Hints: %d\n", hints->input);
			XSetInputFocus(display, trans, RevertToParent, CurrentTime);
	}
	if(child_name!=NULL){
	  if(strcmp(child_name, "Parent") && local_event.xmap.override_redirect == False){
		Window new_win = draw_window_with_name(display, RootWindow(display, infos.screen_num), "Parent", infos.screen_num, 
						   win_attr.x, win_attr.y, win_attr.width, win_attr.height+DECORATION_HEIGHT, 0, 
						   BlackPixel(display, infos.screen_num));
		XMapWindow(display, new_win);
		XReparentWindow(display,local_event.xmap.window, new_win,0, DECORATION_HEIGHT);
		set_window_item(local_event.xmap.window, new_win);
		XSelectInput(display, local_event.xmap.window, StructureNotifyMask);
		printf("\tParent window id: %lu\n", new_win);
		put_text(display, new_win, child_name, "9x15", 40, 15, BlackPixel(display,infos.screen_num), WhitePixel(display, infos.screen_num));
		draw_controls(display, new_win, 5, 5, 20, 20, BlackPixel(display, infos.screen_num), WhitePixel(display, infos.screen_num));
		get_system_icon("default.ico", display, new_win);
	  } /*else {
		  XWindowAttributes attributes;
		  Status status = XGetWindowAttributes(display, local_event.xmap.window, &attributes);
		  printf("\tStatus: %d\n", attributes.map_state);
		  printf("\tOverride redirect: %d\n", attributes.override_redirect);
		  //XMapWindow(display, trans);
		  XFetchName(display, trans, &child_name);
		  printf("\tChild name: %s\n", child_name);
		  //XRaiseWindow(local_event.xmap.display, local_event.xmap.window);
		  //XCirculateSubwindows(local_event.xmap.display, local_event.xmap.window, RaiseLowest);
	  }*/
	}
	XFree(child_name);
}

void destroy_notify_handler(XEvent local_event, Display *display){			
	Window window = local_event.xdestroywindow.event;
	printf("Window to destroy: %lu\n", local_event.xdestroywindow.window);
	WindowItem item = get_window_item(local_event.xdestroywindow.window);
	if(item.key!=0){
		printf("Destroying Window %lu - key: %lu\n", item.value,item.key);
		XDestroyWindow(display, item.value);
		destroy_window_item(item.key);
	}
	
}

void client_message_handler(XEvent local_event, Display *display){
	printf("----------ClientMessage\n");
	printf("\tMessage: %s\n", XGetAtomName(display,local_event.xclient.message_type));
	printf("\tFormat: %d\n", local_event.xclient.format); 
	Atom *atoms = (Atom *)local_event.xclient.data.l;
	int i =0;
	for(i=0; i<=5; i++){
		printf("\t\tData %d: %s\n", i, XGetAtomName(display, atoms[i]));
	}
	int nchild;
	Window *child_windows;
	Window parent_window;
	Window root_window;
	XQueryTree(display, local_event.xclient.window, &root_window, &parent_window, &child_windows, &nchild);
	printf("\tNumber of childs: %d\n", nchild);
	XEvent new_event;
	Atom new_atom = XInternAtom(display, "_NET_WM_STATE_ABOVE", True);
	if(new_atom == False){
		printf("\tDoes not exist _NET_WM_STATE");
	}
	printf("Creating new Atom client Message, with value: %d\n", ClientMessage);
	new_event.xclient.type = ClientMessage;
	new_event.xclient.serial = 0;
	new_event.xclient.send_event = 1;
	new_event.xclient.message_type = XInternAtom(display, "_NET_WM_STATE", False);
	new_event.xclient.display = local_event.xclient.display;
	new_event.xclient.window = local_event.xclient.window;
	new_event.xclient.format = 32;
	new_event.xclient.data.l[0] = 1;
	new_event.xclient.data.l[1] = new_atom;
	new_event.xclient.data.l[2] = 0;
	new_event.xclient.data.l[3] = 0;
	new_event.xclient.data.l[4] = 0;
	unsigned long mask = 1 << 20 /* SubstructureRedirectMask */ | 1 << 19 /* SubstructureNotifyMask */ ;
	XSendEvent(display, InputFocus, False, mask, &new_event);
	XFlush(new_event.xclient.display);
	printf("-------------\n");
}
