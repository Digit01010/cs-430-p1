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
void readP3(Pixel *, Header, FILE *);
void writeP3(Pixel *, Header, FILE *);
void readP6(Pixel *, Header, FILE *);
void writeP6(Pixel *, Header, FILE *);

int main(int argc, char *argv[]) {
  if (argc != 4) {
    fprintf(stderr, "Error: Incorrect number of arguments.\n");
    printf("Usage: ppmrw outputMagicNumber inputFile outputFile\n");
    return(1);
  }
  
  char outMagicNumber;
  if (strcmp(argv[1], "3") == 0) {
    outMagicNumber = 3;
  }
  else if (strcmp(argv[1], "6") == 0) {
    outMagicNumber = 6;
  }
  else {
    fprintf(stderr, "Error: Output magic number not supported.\n");
    return 1;
  }
  
  FILE* input = fopen(argv[2], "r");
  Header inHeader = parseHeader(input);
  
  if (header.maxColor > 255) {
    fprintf(stderr, "Error: Maximum color greater than 255 not supported.\n");
    return 1;
  }
  
  Pixel *buffer = malloc(sizeof(Pixel) * inHeader.width * inHeader.height);
  if (inHeader.magicNumber == 3) {
    readP3(buffer, inHeader, input);
  }
  else if (inHeader.magicNumber == 6) {
    readP6(buffer, inHeader, input);
  }
  fclose(input);
  
  Header outHeader = inHeader;
  outHeader.magicNumber = outMagicNumber; 
  
  FILE* output = fopen(argv[3], "w");
  if (outMagicNumber == 3) {
    writeP3(buffer, outHeader, output);
  }
  else if (outMagicNumber == 6) {
    writeP6(buffer, outHeader, output);
  }
  else {
    fprintf(stderr, "Error: Programmer forgot to support valid output magic number.\n");
    return 1;
  }
  fclose(output);
  
  
  return 0;
}

Header parseHeader(FILE *fh) {
  Header h;
  
  if (fgetc(fh) != 'P') {
    exit(1);
  }
  
  
  fscanf(fh, "%d ", &h.magicNumber);
  
  char c = fgetc(fh);
  while (c == '#') {
    while (fgetc(fh) != '\n') {}
    c = fgetc(fh);
  }
  ungetc(c, fh);
  
  fscanf(fh, "%d ", &h.width);
  
  c = fgetc(fh);
  while (c == '#') {
    while (fgetc(fh) != '\n') {}
    c = fgetc(fh);
  }
  ungetc(c, fh);
  
  fscanf(fh, "%d ", &h.height);
  
  c = fgetc(fh);
  while (c == '#') {
    while (fgetc(fh) != '\n') {}
    c = fgetc(fh);
  }
  ungetc(c, fh);
  
  fscanf(fh, "%d", &h.maxColor);
  fgetc(fh);
   
  return h;
}

void readP3(Pixel *buffer, Header h, FILE *fh) {
  for (int i = 0; i < h.width * h.height; i++) {
     fscanf(fh, "%d %d %d", &buffer[i].red, &buffer[i].green, &buffer[i].blue);
  }
}

void writeP3(Pixel *buffer, Header h, FILE *fh) {
  fprintf(fh, "P%d\n%d %d\n%d\n", h.magicNumber, h.width, h.height, h.maxColor);
  for (int i = 0; i < h.width * h.height; i++) {
     fprintf(fh, "%d\n%d\n%d\n", buffer[i].red, buffer[i].green, buffer[i].blue);
  }
}

void readP6(Pixel *buffer, Header h, FILE *fh) {
  for (int i = 0; i < h.width * h.height; i++) {
     buffer[i].red = fgetc(fh);
     buffer[i].green = fgetc(fh);
     buffer[i].blue = fgetc(fh);
  }
}

void writeP6(Pixel *buffer, Header h, FILE *fh) {
  fprintf(fh, "P%d\n%d %d\n%d\n", h.magicNumber, h.width, h.height, h.maxColor);
  for (int i = 0; i < h.width * h.height; i++) {
     fputc(buffer[i].red, fh);
     fputc(buffer[i].green, fh);
     fputc(buffer[i].blue, fh);
  }
}


