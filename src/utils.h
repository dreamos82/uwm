#ifndef _utils_h
#define _utils_h

#include "definitions.h"

void print_informations(Display *display);
ScreenInfos get_screen_informations(Display * display);
void atom_handler(Display*, Window, Atom);

#endif
