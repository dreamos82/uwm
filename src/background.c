#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>

#include <unistd.h>

#include <X11/Xlib.h>
#include <X11/X.h>
#include <X11/Xutil.h>

#include <Imlib2.h>

#include "definitions.h"
#include "background.h"
#include "utils.h"


void set_window_background(Display *display, GC *gc, char* image_path, Window window){
  ScreenInfos infos = get_screen_informations(display);
  unsigned int img_h;
  unsigned int img_w;
  unsigned int hotspot_x;
  unsigned int hotspot_y;
  Pixmap my_pix;
  Screen *screen = DefaultScreenOfDisplay(display);
  char *filename="test2.jpg";
  Imlib_Image img;
  img = imlib_load_image(filename);
  imlib_context_set_image(img);
  img_w = imlib_image_get_width();
  img_h = imlib_image_get_height();
  my_pix = XCreatePixmap(display, window, img_w, img_h,
        DefaultDepthOfScreen(screen));
        
  imlib_context_set_display(display);
  imlib_context_set_visual(DefaultVisualOfScreen(screen));
  imlib_context_set_colormap(DefaultColormapOfScreen(screen));
  imlib_context_set_drawable(my_pix);
  imlib_render_image_on_drawable(0, 0);
  XSetWindowBackgroundPixmap(display, window, my_pix);
  XClearWindow(display, window);
}
