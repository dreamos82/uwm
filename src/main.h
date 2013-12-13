#ifndef _main_h
#define _main_h

#define NAME "uwm"
#define VERSION "0.1"

#ifdef __FreeBSD__
  #define APP_PATH "/usr/local/bin/xterm"
#endif

#ifdef __linux__
  #define APP_PATH "/usr/bin/xterm"
#endif

#endif
