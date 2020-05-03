#include <stdio.h>
#include <stdlib.h>
#include <X11/Xlib.h>
#include <X11/X.h>
#include <X11/Xutil.h>

#include "definitions.h"
#include "error.h"

int _X_error_handler(Display *display, XErrorEvent *event){
  char error_message[ERROR_STRING_SIZE];
  XGetErrorText(display, event->error_code, error_message, ERROR_STRING_SIZE);
  printf("Error occurred: %s\n", error_message);
}

int _X_io_error_handler(Display *display){
	printf("Fatal error\n");
	exit(1);
}
