USELESS Window Manager Experiments
==================================

Actually This is not a window manager :) . Hopefully one day it will be a minimalistic window manager.

It contains just experiments on xlib development. 

Actually you can find experiments on:

1. Basic event handling
2. Basic keyboard handling
3. Window creation
4. Setting color background
5. Showing Text

Dependencies
------------
In order to compile this program needs the following packages:

* libx11-dev

Compile and launch
------------------

Actually there are no compilation script, so it must be done manually:

	# gcc main.c utils.c handlers.c window.c launcher.c -lX11 -o main
	
And then you can launch it with:

	# xinit ./main -- :1

(With that command a new X server will be launched, and you don't need to close the running server)

