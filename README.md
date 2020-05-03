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
6. Setting root window background image
7. Decorating window.

Dependencies
------------
In order to compile this program, you need the following packages installed:

* libx11-dev
* libimlib2-dev

To run correctly it also needs the following program installed:

* xterm

(since it is the only program launched by this useless window manager)

If you want to use Xephyr to test the window manager you need the package: 

* xserver-xephyr (ubuntu) 

installed.

Compile and launch
------------------

Actually there are no compilation script, so it must be done manually:

	
```console
	gcc main.c utils.c handlers.c window.c launcher.c error.c background.c manager.c -lImlib2 -lX11 -o main
```
	

And then you can launch it in two different ways: 

### Using Xephyr
1. The first is using xephyr,  a tool that launch a X server instance in a window. After installing it (check your distro documentation) type

```console
	Xephyr -screen 1024x768 -br :1
```
Where -screen XxY is the desired resolution. And then you can launch the Window Manager with the following command:

```console
	DISPLAY=:1 ./main [ background ]

```
### Launching Full screen
This method is very useful if you want to debug the window manager.

2. The second method is just launching a new server instance, using the following command:

```console
	xinit ./main [ background ] -- :1
```	
and then you can move between X instances using Ctrl + Alt + Fx

[ background ] is optional, and it specifies the background image. If you don't specify it, a default color will be choosed.

(Using that command a new X instance will be launched, and you don't need to close the running server)

Tested image formats:

* jpg 

Please notice that the background argument is optional. 

Usage instruction
-----------------
There are very few thing that you can do using this useless window manager, here a list of what you can do:

* Press 1 to launch xterm (and with xterm you can launch other programs)
* To close a program just right click on program window decoration.

TODO
----

* Add a window destroy event handler (DONE)
* Basic configuration handling (background color/image and default terminal)
* Scale background image
* Finish work on text drawing functions
* Add better argument parsing.
* Fix issue #2

