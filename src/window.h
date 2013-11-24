#ifndef _window_h
#define _window_h

Window draw_window(Display *, Window parent_window, int, int, int, int, int, int);
Window draw_window_with_name(Display *, Window parent_window, char *,int, int, int, int, int, int);
void set_window_color(Display *, Window, char* );
void set_window_background(Display *, GC*, char*, Window);
void set_window_input_mask(Display *, Window, long);
void put_text(Display*, Window, GC, char*, char*, int, int, ScreenInfos);
#endif
