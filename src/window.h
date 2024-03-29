#ifndef _window_h
#define _window_h

#define CONTROL_LEFT 3
#define CONTROL_RIGHT 1
#define CONTROL_CENTER 2

#define CONTROL_MARGIN_H 30

#define CONTROL_MARGIN_V 3
#define CONTROL_SPACING 10

Window draw_window(Display *, Window parent_window, int, int, int, int, int, int);
Window draw_window_with_name(Display *, Window parent_window, char *,int, int, int, int, int, int, unsigned long);
void set_window_color(Display *, Window, char* );
void set_window_input_mask(Display *, Window, long);
void draw_controls(Display*, Window, XWindowAttributes, unsigned long, unsigned long, unsigned short);
void put_text(Display*, Window, char*, char*, int, int, unsigned long, unsigned long);
#endif
