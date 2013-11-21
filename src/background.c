#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>

#include <unistd.h>

#include <X11/Xlib.h>
#include <X11/X.h>
#include <X11/Xutil.h>

#include <png.h>
//#include <magick/MagickCore.h>

#include "background.h"

static png_structp png_ptr;
static png_infop info_ptr;
unsigned int width, height;
png_bytep * row_pointers;
png_byte number_of_channels;

png_bytep* read_png_image(char *path, unsigned int *w, unsigned int *h){
  printf("Reading background image: %s\n", path);
  printf("Using libpng version: %s\n",PNG_LIBPNG_VER_STRING);
  FILE *fp = fopen(path, "rb");
  if(!fp){
    printf("Cannot open file... %s", path);
  }  
  unsigned int bit_depth, color_type;
  int number_of_passes;
  read_png_init(fp, &width, &height, &bit_depth, &color_type);
  printf("Size: %dx%d Depth: %d Color Type: %d\n", width,height,bit_depth,color_type);
  number_of_passes = png_set_interlace_handling(png_ptr);
  png_read_update_info(png_ptr, info_ptr);
  if (setjmp(png_jmpbuf(png_ptr))){
     printf("[read_png_file] Error during read file");
  }
  number_of_channels = png_get_channels(png_ptr, info_ptr);
  int y;
  row_pointers = (png_bytep*) malloc(sizeof(png_bytep) * height);
  for (y=0; y<height; y++){
    row_pointers[y] = (png_byte*) malloc(png_get_rowbytes(png_ptr,info_ptr));
  }
  png_read_image(png_ptr, row_pointers);
  fclose(fp);
  *w=width;
  *h=height;
  return row_pointers;
}

int read_png_init(FILE* input, unsigned int *width, unsigned int *height, int *bit_depth, int *color_type){
  unsigned char sig[8]; 	
  fread(sig, 1, 8, input);
  if(!png_check_sig(sig, 8)){
    printf("Not a valid png");
    return 1;
  }
  png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
  if (!png_ptr){
      return 4;   /* out of memory */
  }
  
  info_ptr = png_create_info_struct(png_ptr);
  if (!info_ptr) {
    png_destroy_read_struct(&png_ptr, NULL, NULL);
    return 4;   /* out of memory */  
  }
  if (setjmp(png_jmpbuf(png_ptr))){
     printf("[read_png_file] Error during init_io");
  }
  
  png_init_io(png_ptr, input);
  png_set_sig_bytes(png_ptr, 8);
  png_read_info(png_ptr, info_ptr);
  png_get_IHDR(png_ptr, info_ptr, width, height, bit_depth, color_type, NULL, NULL, NULL);
  return 0;
}

char *convert_pngoutput(png_bytep* input, char* output){
  char *buffer = malloc(width*height*8);
  memset(buffer,0,width * height);    
  int i,j;
  int byte_col;
  i=0;  
  byte_col=0;
  while(i<height){
    j=0;
    while(j<width){
      png_byte *row = input[j];
      memcpy(&buffer[byte_col], &input[j], 1);
      j++;
    }
    byte_col += 4;
    i++;
  }
  return buffer;
}

/*int main(){
   ExceptionInfo
      *exception;

    Image
      *image,
      *images,
      *resize_image,
      *thumbnails;

    ImageInfo
      *image_info;
      
      MagickCoreGenesis(*argv,MagickTrue);
    exception=AcquireExceptionInfo();
    image_info=CloneImageInfo((ImageInfo *) NULL);
    (void) strcpy(image_info->filename,"background.png");
    images=ReadImage(image_info,exception);
    
    if (exception->severity != UndefinedException)
      CatchException(exception);
    if (images == (Image *) NULL)
      exit(1);
    while ((image=RemoveFirstImageFromList(&images)) != (Image *) NULL)
    {
	int lenght;
	unsigned char *output = ImagesToBlob(image_info, images,&length, exception);
    }
      /*unsigned int w,h;
  printf("Trying to load a png...");
  png_bytep *output = read_png_image("./background.png", &w, &h);
  printf("SizE: %d\n", sizeof(output[0]));
  char *buffer;
  convert_pngoutput(output, buffer);
}*/
