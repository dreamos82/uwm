#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include <unistd.h>
#include <errno.h>
#include <X11/Xlib.h>
#include <X11/X.h>
#include <X11/Xutil.h>

#include "handlers.h"
#include "main.h"

void button_handler(XEvent event){
  switch(event.xbutton.button){
    case Button1:
      printf("Left Button pressed\n");
      break;
    case Button2:
      printf("Mid Button pressed\n");
      break;
    case Button3:
      printf("Right Button pressed\n");      
      Window current_window = event.xbutton.subwindow;
      char *window_name;
      if(current_window!=None){
	XFetchName(event.xbutton.display, current_window, &window_name);
      } else {
	XFetchName(event.xbutton.display, event.xbutton.window, &window_name);
      }
      printf("Click on: %s", window_name);
      break;
  }
}

char keyboard_handler(XEvent event, Display* display){
  int keysyms_per_keycode_return;
  KeySym key_symbol = XGetKeyboardMapping(display,event.xkey.keycode,1, &keysyms_per_keycode_return);
  if(XLookupKeysym(&event.xkey, 0)== XK_1){
    printf("ci sono\n");
    int pid = fork();
    if(pid==0) {      
	if(execlp("xterm", "xterm", "-ut", 0)==-1){
	  printf("%s", strerror(errno));
	}
    }
  }
  int ascii_key = key_symbol - XK_A + 'A';
  //printf("Key pressed - '%c'\n", ascii_key);
}