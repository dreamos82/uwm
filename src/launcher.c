#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include <X11/Xlib.h>
#include <X11/X.h>
#include <X11/Xutil.h>

#include "definitions.h"
#include "window.h"
#include "launcher.h"

void create_launcher(Display *display, Window parent_window, ScreenInfos infos){
  Window window = draw_window_with_name(display, parent_window, "Application Launcher",infos.screen_num, infos.width/2, infos.height/2, 300, 150, 30, WhitePixel(display, infos.screen_num));
  XSelectInput(display, window, ExposureMask | ButtonPressMask | KeyPressMask);
  XGCValues     values;
  GC gc = XCreateGC(display, window, 0, &values);
  //put_text(display, window, gc, "Test", "9x15", infos);
  XMapWindow(display, window);
}
