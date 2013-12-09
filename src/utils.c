#include <stdio.h>
#include <stdlib.h>
#include <X11/Xlib.h>
#include <X11/X.h>
#include <X11/Xutil.h>

#include "definitions.h"
#include "utils.h"

void print_informations(Display *display){
	int screen_width;
	int screen_num;
	int screen_height;
	int number_of_screens;
	
	printf("Screen informations...\n");
	screen_num = DefaultScreen(display);
	printf("Screen Number: %d\n", screen_num);	
	screen_width = DisplayWidth(display, screen_num);
	printf("Screen Width: %d\n", screen_width);
	screen_height = DisplayHeight(display, screen_num);
	printf("Screen Height: %d\n", screen_height);
	number_of_screens = XScreenCount(display);			
	printf("Number of screens: %d\n", number_of_screens);
}

ScreenInfos get_screen_informations(Display *display){
	ScreenInfos infos;
	int screen_num = DefaultScreen(display);
	infos.screen_num = screen_num;
	infos.width = DisplayWidth(display,screen_num);
	infos.height = DisplayHeight(display, screen_num);
	infos.number_of_screens = XScreenCount(display);
	return infos;
}
