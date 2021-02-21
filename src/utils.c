#include <stdio.h>
#include <stdlib.h>
#include <X11/Xlib.h>
#include <X11/X.h>
#include <X11/Xutil.h>
#include <X11/Xatom.h>

#include "definitions.h"
#include "utils.h"

void print_informations(Display *display){
    int screen_width;
    int screen_num;
    int screen_height;
    int number_of_screens;

    printf("Screen informations...\n");
    screen_num = DefaultScreen(display);
    printf("Screen Number: %d\n", screen_num);	
    screen_width = DisplayWidth(display, screen_num);
    printf("Screen Width: %d\n", screen_width);
    screen_height = DisplayHeight(display, screen_num);
    printf("Screen Height: %d\n", screen_height);
    number_of_screens = XScreenCount(display);			
    printf("Number of screens: %d\n", number_of_screens);
}

ScreenInfos get_screen_informations(Display *display){
    ScreenInfos infos;
    int screen_num = DefaultScreen(display);
    infos.screen_num = screen_num;
    infos.width = DisplayWidth(display,screen_num);
    infos.height = DisplayHeight(display, screen_num);
    infos.number_of_screens = XScreenCount(display);
    return infos;
}

unsigned long get_window_pid(Display *display, Window window){
    int result;
    int _format;
    unsigned long _nitems;
    unsigned long _bytes_after;
    unsigned char *prop_return=0;
    unsigned int _no_items=0;
    Window parent;
    Window root_return;
    Window *childrens;
    Atom pid_atom = XInternAtom(display, "_NET_WM_PID", True);
    Atom type;
    if(pid_atom==None) { 
        printf("Pid not found\n");
        return 0;
    } else {
        printf("Something found\n");
        if(XGetWindowProperty(display, window, pid_atom,0,1,False,XA_CARDINAL,&type, &_format, &_nitems, &_bytes_after,&prop_return) == Success){
            int result = 0;	
            printf("Request done \n");
            XQueryTree(display, window, &root_return, &parent, &childrens, &_no_items);
            if(_no_items>0){
                result = get_window_pid(display, childrens[0]);
            }
            printf("No_items: %d\n", _no_items);
            if(result == 0 && type==None) {
                printf("Property Not found");
                printf("Format: %d\n", _format);
                printf("Bytes_after: %ld\n", _bytes_after);
                return 0;
            }
            if(prop_return!=0){
                printf("- Not zero\n");
                printf("Propid: %lu\n", *((unsigned long *)prop_return));
                return *((unsigned long *)prop_return);
            }
            //unsigned long _pid= *((unsigned long *)prop_return);
            //XFree(prop_return);
            return result;
        }
    }
    return 0;
}

int window_get_name(Display *dpy, Window w, char **name) {
    Atom prop = XInternAtom(dpy, "_NET_WM_NAME", False);
    Atom utf8_type = XInternAtom(dpy, "UTF8_STRING", False);
    Atom actual_type;
    int actual_format;
    unsigned long nitems;
    unsigned long leftover;
    char *data = NULL;
    Status ret;

    if (Success != (ret = XGetWindowProperty(dpy, w, prop, 0L, (long) BUFSIZ,
                    False, utf8_type, &actual_type, &actual_format, &nitems,
                    &leftover, (unsigned char **) &data))) {
        if (BadWindow == ret)
            return 0;
        printf("Window %#010lx: _NET_WM_NAME unset, falling back to WM_NAME.\n", w);
        if (!XFetchName(dpy, w, &data))
            return 0;
    }
    // if (actual_type == utf8_type && actual_format == 8)
    *name = (char *) data;
    return 1;
}

int get_property_value(Display* display, Window window,char *propname, long max_length,
        unsigned long *nitems_return, unsigned char **prop_return){
    int result;
    Atom property;
    Atom actual_type_return;
    int actual_format_return;
    Window *transient_to_return;
    unsigned long bytes_after_return;
    unsigned char* prop_to_return;
    unsigned long n_items;
    //int max_length = 32;
    printf("----GET_PROPERTY_VALUE----\n");
    printf("\tPropname: %s\n", propname);
    property = XInternAtom(display, propname, True);
    if(property==None){
        printf("\tWrong Atom\n");
        return None;
    }

    result = XGetWindowProperty(display, window, property, 0,	/* long_offset */
            (~0L),	/* long_length */
            False,	/* delete */
            AnyPropertyType,	/* req_type */
            &actual_type_return,
            &actual_format_return,
            &n_items, &bytes_after_return, &prop_to_return);
    if (result != Success){
        printf("\tXGetWindowProperty failed\n");
        return (-1);
    } 

    if (actual_type_return == None) {
        printf("\tActualTypeReturn is None\n");
        return (-1);
    } else {
        printf("\tActual Type: %s\n", XGetAtomName(display,property));
        printf("\tProperty format: %d\n", actual_format_return);
        //printf("Actual property return: %s\n", XGetAtomName(display,actual_type_return));
        printf("\tByte after return: %ld\n", bytes_after_return);
        printf("\tnitems return: %d\n", n_items);
        printf("\tprop return: %s\n", prop_to_return);
    }
    printf("-----END OF GET_PROPERTY_VALUE-------\n");

    return (0);
}
