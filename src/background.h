#ifndef _background_h
#define _background_h

png_bytep* read_png_image(char *, unsigned int*, unsigned int*);
int read_png_init(FILE*, unsigned int *, unsigned int *, int *, int *);
int readpng_get_bgcolor(unsigned char *, unsigned char *, unsigned char *);
char *convert_pngoutput(png_bytep*, char*);
unsigned char* getBytes();
#endif
