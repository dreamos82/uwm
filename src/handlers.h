#ifndef _handlers_h
#define _handlers_h

void button_handler(XEvent, Display*, ScreenInfos);
char keyboard_handler(XEvent, Display *);
void motion_handler(XEvent, Display *);
void configure_notify_handler(XEvent, Display *);
void map_notify_handler(XEvent, Display *, ScreenInfos);
void create_notify_handler(XEvent, Display *);
void destroy_notify_handler(XEvent, Display *);
void client_message_handler(XEvent, Display *);
void expose_hanlder(XEvent, Display *);
#endif
