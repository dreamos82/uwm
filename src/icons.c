#include <stdio.h>
#include <stdlib.h>
#include <X11/X.h>
#include <X11/Xutil.h>
#include <X11/Xlib.h>
#include <Imlib2.h>

#include "icons.h"
#include "utils.h"

void draw_application_icon(char* icon_name, Display *display, Window win, unsigned int x, unsigned int y){
    unsigned int img_h;
    unsigned int img_w;
    Pixmap my_pix;
    Imlib_Image img;

    img = imlib_load_image(icon_name);
    if(img == NULL){
        printf("Error loading imlibimage - icon file: %s", icon_name);
    }
    Screen *screen = DefaultScreenOfDisplay(display);
    imlib_context_set_image(img);
    img_w = imlib_image_get_width();
    img_h = imlib_image_get_height();

    Window icon_window; 
    icon_window = XCreateSimpleWindow(display, win, x, y, img_w, img_h, 1, WhitePixel(display, 0), WhitePixel(display, 0));

    printf("Img size: %d - %d\n", img_w, img_h);
    my_pix = XCreatePixmap(display, icon_window, img_w, img_h, DefaultDepthOfScreen(screen));
    imlib_context_set_display(display);
    imlib_context_set_visual(DefaultVisualOfScreen(screen));
    imlib_context_set_colormap(DefaultColormapOfScreen(screen));
    imlib_context_set_drawable(my_pix);
    imlib_render_image_on_drawable(0, 0);
    XSetWindowBackgroundPixmap(display, icon_window, my_pix);
    XMapWindow(display, icon_window);
    XClearWindow(display, win);
    XFreePixmap(display, my_pix);
    imlib_free_image();
}

