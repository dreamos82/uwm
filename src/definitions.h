#ifndef _definitions_h
#define _definitions_h

#include <X11/Xlib.h>

#define BORDER_WIDTH 5
#define BORDER_NONE 0

#define DECORATION_HEIGHT 30

#define SIZE_OF_LIST 20

#define DEFAULT_TERMINAL "xterm"

#define _NET_WM_STATE_TOGGLE 2

#define ERROR_STRING_SIZE 100

typedef struct {
	int screen_num;
	int width;
	int height; 
	int number_of_screens;
} ScreenInfos;

typedef struct { 
	Window key;
	Window value;
} WindowItem;

#endif
