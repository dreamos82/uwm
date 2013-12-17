#ifndef _manager_h
#define _manager_h

#include "definitions.h"

extern WindowItem WindowsList[SIZE_OF_LIST];
WindowItem get_window_item(Window);
void set_window_item(Window, Window);
void destroy_window_item(Window);
#endif
