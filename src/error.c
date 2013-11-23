#include <stdio.h>
#include <X11/Xlib.h>
#include <X11/X.h>
#include <X11/Xutil.h>

#include "definitions.h"
#include "error.h"

int _X_error_handler(Display *display, XErrorEvent *event){
  char error_message[100];
  XGetErrorText(display, event->error_code, error_message, 100);
  printf("Error occurred: %s", error_message);
}