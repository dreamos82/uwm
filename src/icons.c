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
		imlib_blend_image_onto_image(img,0,0,0, img_w, img_h, 0,0, infos.width,infos.height);
		Pixmap my_pix;
		my_pix = XCreatePixmap(display, win, img_w, img_h, DefaultDepthOfScreen(screen));
		imlib_context_set_display(display);
		imlib_context_set_visual(DefaultVisualOfScreen(screen));
		imlib_context_set_colormap(DefaultColormapOfScreen(screen));
		imlib_context_set_drawable(my_pix);
		imlib_render_image_on_drawable(0, 0);
 
		XSizeHints* win_size_hints;
		XWMHints* win_hints;
		int return_code;
/*		return_code = XReadBitmapFile(display, win,
                         filename,
                         &width, &height,
                         &icon,
                         &h_x, &h_y);*/
		/* check for failure or success. */
/*		switch (return_code) {
		    case BitmapOpenFailed:
		        fprintf(stderr, "XReadBitmapFile - could not open file 'icon.bmp'.\n");
		        break;
		    case BitmapFileInvalid:
		        fprintf(stderr,
		                "XReadBitmapFile - file '%s' doesn't contain a valid bitmap.\n",
		                filename);
		        break;
		    case BitmapNoMemory:
		        fprintf(stderr, "XReadBitmapFile - not enough memory.\n");
		        break;
		    case BitmapSuccess:
		        bitmap loaded successfully - do something with it... 
				printf("Bitmap loaded correctly\n");
		        break;
		}*/

		return_code = XStringListToTextProperty(&icon_name,
                                   1,
                                   &icon_name_property);
		if(return_code == 0) {
			printf("Error");
		}
		XSetWMIconName(display, win, &icon_name_property);
		win_hints = XAllocWMHints();
	    if (!win_size_hints) {
    	    fprintf(stderr, "XAllocSizeHints - out of memory\n");
        	exit(1);
	    }
		win_hints->flags = IconPixmapHint | StateHint | IconPositionHint;
	    win_hints->icon_pixmap = my_pix;
    	win_hints->initial_state = IconicState;
	    win_hints->icon_x = 0;
    	win_hints->icon_y = 0;

	    /* pass the hints to the window manager. */
	    XSetWMHints(display, win, win_hints);

    	/* finally, we can free the WM hints structure. */
		GC gc;
		gc = XCreateGC(display, win, 0, NULL);
		//XPutImage(display, win, gc, 
 	   	XSetBackground(display, gc, WhitePixel(display, DefaultScreen(display)));	
		//WhitePixel(display, DefaultScreen(display));

		XCopyPlane(display, my_pix, win, gc,
                    0, 0,
                    30, 30,
                    0, 0,
                    1);	
	
	    XFree(win_hints);
	XFlush(display);
	
}

