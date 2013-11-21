/*
	  HelloX.C
	 "Hello, World!", X Window System style.
*/


#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xos.h>
#include <X11/Xatom.h>

#include <stdlib.h>
#include <string.h>
#include <stdio.h>


/*  Global variables  */

Display *     display;
int           screen_num;
static char * appname;


/*  main() function  */

int main( int argc, char * argv[] ) {

    /*  Window variables  */

    Window       win, root_window;
    int          x, y;
    
    unsigned int width, height;
    unsigned int border_width;
    char *       window_name = "Xlib Example";
    char *       icon_name   = "Xlib Example";

    /*  Mouse-keyboard variables  */
    int          xM, yM;
    int          bufferlength = 64;
    char    	 buffer[65];
    KeySym  	 keysym;

    /*  Display variables  */

    char *        display_name = NULL;
    unsigned int  display_width, display_height;
    unsigned long white_pixel,  black_pixel;    

    /*  Color variables  */
    Colormap  colorMap;  /* color map to use for allocating colors. */
    XColor    red, blue; /* used for allocation of the given color map entries. */

    /*  Miscellaneous X variables  */

    XSizeHints *  size_hints;
    XWMHints   *  wm_hints;
    XClassHint *  class_hints;
    XTextProperty windowName, iconName;
    XEvent        report;
    XFontStruct * font_info;
    XGCValues     values;
    GC            gc;
    
    appname = argv[0];

    /*  Allocate memory for our structures  */

    if ( !( size_hints  = XAllocSizeHints() ) || 
	 !( wm_hints    = XAllocWMHints()   ) ||
	 !( class_hints = XAllocClassHint() )    ) {
	fprintf(stderr, "%s: couldn't allocate memory.\n", appname);
	exit(EXIT_FAILURE);
    }

    /*  Connect to X server  */

    if ( (display = XOpenDisplay(display_name)) == NULL ) {
	fprintf(stderr, "%s: couldn't connect to X server %s\n", appname, display_name);
	exit(EXIT_FAILURE);
    }


    /*  Get screen size from display structure macro  */

    screen_num     = DefaultScreen(display);
    display_width  = DisplayWidth(display, screen_num);
    display_height = DisplayHeight(display, screen_num);

    root_window    = RootWindow(display, screen_num); /* find the ID of the root window of the screen. */
    white_pixel    = WhitePixel(display, screen_num); /* find the value of a white pixel on this screen. */
    black_pixel    = BlackPixel(display, screen_num); /* find the value of a black pixel on this screen. */ 


    /*  Set initial window size, position and border, and create it  */

    x = y = 100;
    width  = display_width/2;
    height = display_height/2;
    border_width = 2; 


    /* create a simple window, as a direct child of the screen's   */
    /* root window. Use the screen's white color as the background */
    /* color of the window. Place the new window's top-left corner */
    /* at the given 'x,y' coordinates.                             */

    win = XCreateSimpleWindow(display, root_window,
			      x, y, width, height, border_width,
			      white_pixel,white_pixel);


    /*  Set hints for window manager before mapping window  */

    if ( XStringListToTextProperty(&window_name, 1, &windowName) == 0 ) {
	fprintf(stderr, "%s: structure allocation for windowName failed.\n", appname);
	exit(EXIT_FAILURE);
    }

    if ( XStringListToTextProperty(&icon_name, 1, &iconName) == 0 ) {
	fprintf(stderr, "%s: structure allocation for iconName failed.\n", appname);
	exit(EXIT_FAILURE);
    }

    size_hints->flags       = PPosition | PSize | PMinSize;
    size_hints->min_width   = 100;
    size_hints->min_height  = 100;

    wm_hints->flags         = StateHint | InputHint;
    wm_hints->initial_state = NormalState;
    wm_hints->input         = True;

    class_hints->res_name   = appname;
    class_hints->res_class  = "xlib_example";

    XSetWMProperties(display, win, &windowName, &iconName, argv, argc,
		     size_hints, wm_hints, class_hints);


    /*  Display Window  */

    XMapWindow(display, win);

    /*  Choose which events we want to handle  */

    XSelectInput(display, win, 
    		 ExposureMask | KeyPressMask | ButtonPressMask | StructureNotifyMask);


    /*  Load a font called "9x15"  */
    if ( (font_info = XLoadQueryFont(display, "9x15")) == NULL ) {
	fprintf(stderr, "%s: cannot open 9x15 font.\n", appname);
	exit(EXIT_FAILURE);
    }
    /*  Create graphics context  */
    gc = XCreateGC(display, win, 0, &values);
    
    
    XSetFont(display, gc, font_info->fid);
    XSetBackground(display, gc, white_pixel);

    /* get access to the screen's color map. */
    colorMap = DefaultColormap(display, screen_num);

    /* allocate the set of colors we will want to use for the drawing. */  
    if (! XAllocNamedColor(display, colorMap, "red", &red, &red)) {
    	fprintf(stderr, "XAllocNamedColor - failed to allocated 'red' color.\n");
    	exit(1);
    }
    if (! XAllocNamedColor(display, colorMap, "blue", &blue, &blue)) {
    	fprintf(stderr, "XAllocNamedColor - failed to allocated 'blue' color.\n");
    	exit(1);
    }

    
        
    /*  Enter event loop  */

    while ( 1 ) {
	static char * message = "Hello, X Window System!";
	static int    length;
	static int    font_height;
	static int    msg_x, msg_y;

	XNextEvent(display, &report);

	switch ( report.type ) {

	case Expose:

	    if ( report.xexpose.count != 0 ) break;

	    /*  Redraw window */

	    /*  Output message centrally in window  */

	    length = XTextWidth(font_info, message, strlen(message));
	    msg_x  = (width - length) / 2;

	    font_height = font_info->ascent + font_info->descent;
	    msg_y  = (height + font_height) / 2;

	    /* draw the message in a red color. */
	    XSetForeground(display, gc, red.pixel);
	    XDrawString(display, win, gc, msg_x, msg_y,	message, strlen(message));

	    break;


	case ConfigureNotify:

	    /*  Store new window width & height  */

	    width  = report.xconfigure.width;
	    height = report.xconfigure.height;

	    break;

	case KeyPress:
	
	    length = XLookupString(&report.xkey, buffer, bufferlength, &keysym, NULL );
		
	    /* if press 'q', quit... */
	    if(keysym == 'q'){

            	/* Clean up and exit  */
		printf("Quiting...\n");
 	        XUnloadFont(display, font_info->fid);
	        XFreeGC(display, gc);
	        XCloseDisplay(display);
	        exit(EXIT_SUCCESS);

            }  
	    break;
	    
	case ButtonPress:

	  xM = report.xbutton.x;
	  yM = report.xbutton.y;
	  switch (report.xbutton.button) {
	    case Button1:
	      /* draw the point in a black color. */
	      XSetForeground(display, gc, black_pixel);
	      XDrawPoint(display, win, gc, xM, yM);
	      break;

	    case Button2:
	    case Button3: 
	      break;      
	  }
	
	default: /* unknown event type - ignore it. */
            break;
	}
    }

    return EXIT_SUCCESS;   /*  We shouldn't get here  */
}