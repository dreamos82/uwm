#ifndef _window_h
#define _window_h

Window draw_window(Display *, Window parent_window, int, int, int, int, int, int);
Window draw_window_with_name(Display *, Window parent_window, char *,int, int, int, int, int, int, unsigned long);
void set_window_color(Display *, Window, char* );
void set_window_input_mask(Display *, Window, long);
void put_text(Display*, Window, char*, char*, int, int, unsigned long, unsigned long);
#endif
