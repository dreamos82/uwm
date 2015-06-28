#include <stdio.h>
#include <stddef.h>

#include <X11/Xlib.h>

#include "manager.h"

WindowItem WindowsList[SIZE_OF_LIST];

void init_manager(){
	int i=0;
	for(i=0; i<SIZE_OF_LIST; i++){
		WindowsList[i].key = 0;
		WindowsList[i].value = 0;
	}
}

WindowItem get_window_item(Window window){
	int i=0;
	for(i=0; i<SIZE_OF_LIST; i++){
		//printf("Window id: %lu ", WindowsList[i].key);
		if(WindowsList[i].key == window){
			printf("Item Found\n");
			return WindowsList[i];
		}
	}
	printf("NotFound\n");
	WindowItem empty_item;
	empty_item.key = 0;
	empty_item.value = 0;
	return empty_item;
}

void set_window_item(Window window, Window parent){
	printf("In set_window_item: %lu\n", parent);
	int i=0;
	while(WindowsList[i].key != 0 && i<SIZE_OF_LIST){
		printf("Cycle\n");
		i++;
	}
	if(i<SIZE_OF_LIST){
		printf("Added");
		WindowsList[i].key = window;
		WindowsList[i].value = parent;
	}
}

void destroy_window_item(Window window){
	int i=0;
	for(i=0; i<SIZE_OF_LIST; i++){
		if(WindowsList[i].key == window){
			WindowsList[i].key = 0;
			WindowsList[i].value = 0;
		}
	}
}
