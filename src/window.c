#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>

#include <X11/Xlib.h>
#include <X11/X.h>
#include <X11/Xutil.h>

#include "definitions.h"
#include "window.h"
#include "utils.h"
#include "icons.h"

Window draw_window(Display *display, Window parent_window, int screen_num,int posx, int posy, int width, int height, int border_width){
	Window window;
	window = XCreateSimpleWindow(display, parent_window, posx, posy, width, height, border_width, BlackPixel(display,screen_num),WhitePixel(display,screen_num));
	return window;
}

Window draw_window_with_name(Display *display, Window parent_window, char* name, int screen_num,int posx, int posy, int width, int height, int border_width, unsigned long back_color){
	Window window;
	window = XCreateSimpleWindow(display, parent_window, posx, posy, width, height, border_width, BlackPixel(display,screen_num),back_color);
	XStoreName(display, window, name);
	return window;
}

void set_window_input_mask(Display *display, Window window, long event_mask){
	XSelectInput(display, window, event_mask);
}

void set_window_color(Display *display, Window window, char* root_color){
	Colormap colormap; 
	XColor color;
	colormap = DefaultColormap(display, 0);
	XParseColor(display, colormap, root_color, &color);
	XAllocColor(display, colormap, &color);
	XSetWindowBackground(display, window, color.pixel);
	XClearWindow(display, window);
}

void draw_controls(Display* display, Window window, int x, int y, int width, int height, unsigned long back_color, unsigned long fore_color){
	Window button_window; 
	button_window = XCreateSimpleWindow(display, window, x, y, width, height, 1, WhitePixel(display, 0), WhitePixel(display, 0));

    get_system_icon("default.ico", display, button_window);
	/*XStoreName(display, button_window, "QuitButton");
	XGCValues values; 
	GC local_gc = XCreateGC(display, button_window, 0, &values);
	XSetBackground(display, local_gc, BlackPixel(display, 0));
	XSetForeground(display, local_gc, BlackPixel(display, 0));
	XFillRectangle(display, button_window, local_gc, x, y, width, height);*/
	XMapWindow(display, button_window);
}

void put_text(Display* display, Window window, char* text, char *font_name, int x, int y, unsigned long back_color, unsigned long fore_color){
  XFontStruct* font_info;
  XGCValues values;	
  GC local_gc = XCreateGC(display, window, 0, &values);
  font_info = XLoadQueryFont(display, font_name);
  if (!font_info) {
    fprintf(stderr, "XLoadQueryFont: failed loading font '%s'\n", font_name);
  }
  int sw;
  sw = XTextWidth(font_info, text, strlen(text));
  //XSetFont(display, gc, font_info->fid);
  XSetBackground(display, local_gc, back_color);
  XSetForeground(display, local_gc, fore_color);
  printf("Drawing: %s %zu\n", text, strlen(text));
  XDrawString(display, window, local_gc, x, y, text, strlen(text));
  XFlush(display);
}
