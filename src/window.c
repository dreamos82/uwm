#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>

#include <X11/Xlib.h>
#include <X11/X.h>
#include <X11/Xutil.h>

//#include <png.h>

#include "definitions.h"
#include "window.h"
//#include "background.h"
#include "utils.h"

Window draw_window(Display *display, Window parent_window, int screen_num,int posx, int posy, int width, int height, int border_width){
	Window window;
	window = XCreateSimpleWindow(display, parent_window, posx, posy, width, height, border_width, BlackPixel(display,screen_num),WhitePixel(display,screen_num));
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


void set_window_background(Display *display, GC *gc, char* image_path, Window window){
  ScreenInfos infos = get_screen_informations(display);
  unsigned int img_h;
  unsigned int img_w;  
  //png_bytep* image_output = read_png_image("./background.png", &img_h, &img_w);    
  /*//char *output_buffer;
  *gc = XCreateGC(display, window, 0,0);
  XImage *img;
  printf("Loading\n");  
  //convert_pngoutput(image_output, output_buffer);  
  //XImage * img = XCreateImage(display, CopyFromParent, 8, ZPixmap, 0, image_output, img_w, img_h, 32, img_w*4);
  printf("Finished\n");
  //XPutImage(display, window, *gc, img, 0,0,0,0, 1920, 1440);*/
}

void put_text(Display* display, Window window, GC gc,char* text, char *font_name, ScreenInfos infos){
  XFontStruct* font_info;  
  font_info = XLoadQueryFont(display, font_name);
  if (!font_info) {
    fprintf(stderr, "XLoadQueryFont: failed loading font '%s'\n", font_name);
  }
  int x, y;
  x=y=0;
  int sw;
  sw = XTextWidth(font_info, text, strlen(text));
  XSetFont(display, gc, font_info->fid);
  XSetBackground(display, gc, WhitePixel(display,0));
  XSetForeground(display, gc, BlackPixel(display,0));
  printf("Drawing: %s %d\n", text, strlen(text));
  XDrawString(display, window, gc, x+50, y+50, text, strlen(text));
  XFlush(display);
}
