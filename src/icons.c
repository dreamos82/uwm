#include <stdio.h>
#include <stdlib.h>
#include <X11/X.h>
#include <X11/Xutil.h>
#include <X11/Xlib.h>
#include <Imlib2.h>

#include "icons.h"
#include "utils.h"

void get_system_icon(char* icon_name, Display *display, Window win){
		unsigned int img_h;
		unsigned int img_w;
		char *filename = "../default.bmp";
		Pixmap my_pix;
  		Imlib_Image img;

  		img = imlib_load_image(filename);
		if(img == NULL){
				printf("Error loading imlibimage");
		}
  		Screen *screen = DefaultScreenOfDisplay(display);
  		imlib_context_set_image(img);
	  	img_w = imlib_image_get_width();
		img_h = imlib_image_get_height();
		printf("Img size: %d - %d\n", img_w, img_h);
		my_pix = XCreatePixmap(display, win, img_w, img_h, DefaultDepthOfScreen(screen));
		imlib_context_set_display(display);
		imlib_context_set_visual(DefaultVisualOfScreen(screen));
		imlib_context_set_colormap(DefaultColormapOfScreen(screen));
		imlib_context_set_drawable(my_pix);
		imlib_render_image_on_drawable(0, 0);

        XSetWindowBackgroundPixmap(display, win, my_pix);
        XClearWindow(display, win);
        XFreePixmap(display, my_pix);
        imlib_free_image();
}

