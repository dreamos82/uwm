#ifndef _utils_h
#define _utils_h

#include "definitions.h"

void print_informations(Display *);
ScreenInfos get_screen_informations(Display * );
void atom_handler(Display*, Window, Atom);
int get_property_value(Display*, char *, long, unsigned long *, unsigned char **);
unsigned long get_window_pid(Display *, Window);

#endif
