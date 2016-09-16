#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef struct Pixel {
  unsigned char red, green, blue;
} Pixel;

typedef struct Header {
   unsigned char magicNumber;
   unsigned int width, height, maxColor;
} Header;

Header parseHeader(FILE *);

int main(int argc, char *argv[]) {
  if (argc != 4) {
    printf("Usage: ppmrw mn input output\n");
    return(1);
  }
  
  FILE* fh = fopen(argv[2], "r");
  Header h = parseHeader(fh);
  printf("%d %d %d %d \n", h.magicNumber, h.width, h.height, h.maxColor);
  fclose(fh);
  return 0;
}

Header parseHeader(FILE *fh) {
  Header header;
  
  if (fgetc(fh) != 'P') {
    exit(1);
  }
  
  fscanf(fh, "%d ", &header.magicNumber);
  fscanf(fh, "%d ", &header.width);
  fscanf(fh, "%d ", &header.height);
  fscanf(fh, "%d ", &header.maxColor);
   
  return header;
}
