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
		unsigned int hotspot_x;
		unsigned int hotspot_y;
 
		Pixmap icon;
		XTextProperty icon_name_property;
		
		int width, height;
		int h_x, h_y;
		char *filename = "../default.bmp";
  		Imlib_Image img;
  		img = imlib_load_image(filename);
		if(img == NULL){
				printf("Error loading imlibimage");
		}
  		ScreenInfos infos = get_screen_informations(display);
  		Screen *screen = DefaultScreenOfDisplay(display);
  		imlib_context_set_image(img);
	  	img_w = imlib_image_get_width();
		img_h = imlib_image_get_height();
		printf("Img size: %d - %d\n", img_w, img_h);
		//imlib_blend_image_onto_image(img,0,0,0, img_w, img_h, 0,0, infos.width,infos.height);
		Pixmap my_pix;
		my_pix = XCreatePixmap(display, win, img_w, img_h, DefaultDepthOfScreen(screen));
		imlib_context_set_display(display);
		imlib_context_set_visual(DefaultVisualOfScreen(screen));
		imlib_context_set_colormap(DefaultColormapOfScreen(screen));
		imlib_context_set_drawable(my_pix);
//        imlib_context_set_cliprect(0,0,img_w,img_h);
		imlib_render_image_on_drawable(0, 0);

        XSetWindowBackgroundPixmap(display, win, my_pix);
        XClearWindow(display, win);

        while (XPending(display)) {
            XEvent ev;
            XNextEvent(display, &ev);
        }
        XFreePixmap(display, my_pix);
        imlib_free_image();
}

