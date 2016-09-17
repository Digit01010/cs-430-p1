#include <stdlib.h>
#include <stdio.h>
#include <string.h>

// Holds an rgb triple of a pixel
typedef struct Pixel {
  unsigned char red, green, blue;
} Pixel;

// Holds information about the header of a ppm file
typedef struct Header {
   unsigned char magicNumber;
   unsigned int width, height, maxColor;
} Header;

// Function declarations
Header parseHeader(FILE *);
void readP3(Pixel *, Header, FILE *);
void writeP3(Pixel *, Header, FILE *);
void readP6(Pixel *, Header, FILE *);
void writeP6(Pixel *, Header, FILE *);
void skipComments(FILE *);

int main(int argc, char *argv[]) {
  if (argc != 4) {
    fprintf(stderr, "Error: Incorrect number of arguments.\n");
    printf("Usage: ppmrw outputMagicNumber inputFile outputFile\n");
    return(1);
  }
  
  // This flow makes argument parsing more flexible, such as supporting
  // an argument of 'P6' rather than '6', though this is not used.
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
  if (input == NULL) {
    fprintf(stderr, "Error: Unable to open input file.");
    return 1;
  }
  
  // Get header information from input file
  Header inHeader = parseHeader(input);
  
  if (inHeader.maxColor > 255) {
    fprintf(stderr, "Error: Maximum color greater than 255 not supported.\n");
    return 1;
  }
  
  // Create buffer and read data from input using appropriate function.
  Pixel *buffer = malloc(sizeof(Pixel) * inHeader.width * inHeader.height);
  if (inHeader.magicNumber == 3) {
    readP3(buffer, inHeader, input);
  }
  else if (inHeader.magicNumber == 6) {
    readP6(buffer, inHeader, input);
  }
  else {
    fprintf(stderr, "Error: Input magic number not supported.\n");
  }
  fclose(input);
  
  // Create header for output file. We only (potentially) change the magic
  // number from the input header to the output header.
  Header outHeader = inHeader;
  outHeader.magicNumber = outMagicNumber; 
  
  FILE* output = fopen(argv[3], "w");
  if (output == NULL) {
    fprintf(stderr, "Error: Unable to open output file.");
    return 1;
  }
  
  // Call appropriate write function to write the header and data.
  if (outMagicNumber == 3) {
    writeP3(buffer, outHeader, output);
  }
  else if (outMagicNumber == 6) {
    writeP6(buffer, outHeader, output);
  }
  else {
    // Since magic number argument is validated before the data is read,
    // this should never be reached unless the programmer forgot to add
    // another case here.
    fprintf(stderr, "Error: Programmer forgot to support valid output magic number.\n");
    return 1;
  }
  fclose(output);
  
  
  return 0;
}

// Parses the data in the header and moves the position to the
// beginning of the data.
Header parseHeader(FILE *fh) {
  Header h;
  
  // Check if there is a magic number
  if (fgetc(fh) != 'P') {
    fprintf(stderr, "Error: Malformed input magic number. \n");
    exit(1);
  }
  
  // Parse magic number
  fscanf(fh, "%d ", &h.magicNumber);
  
  skipComments(fh);
  
  // Parse width
  fscanf(fh, "%d ", &h.width);
  
  skipComments(fh);
  
  // Parse height
  fscanf(fh, "%d ", &h.height);
  
  skipComments(fh);
  
  // Parse maximum color value
  fscanf(fh, "%d", &h.maxColor);
  
  // Skip single whitespace character before data
  fgetc(fh);
  
  // Check if any parsing encountered an error.
  if (ferror(fh) != 0) {
     fprintf(stderr, "Error: Unable to read header.");
     exit(1);
  }
   
  return h;
}

// Reads P3 data
void readP3(Pixel *buffer, Header h, FILE *fh) {
  // Read RGB triples
  for (int i = 0; i < h.width * h.height; i++) {
     fscanf(fh, "%d %d %d", &buffer[i].red, &buffer[i].green, &buffer[i].blue);
  }
  if (ferror(fh) != 0) {
     fprintf(stderr, "Error: Unable to read data.");
     exit(1);
  }
}

// Writes P3 data
void writeP3(Pixel *buffer, Header h, FILE *fh) {
  // Write the header
  fprintf(fh, "P%d\n%d %d\n%d\n", h.magicNumber, h.width, h.height, h.maxColor);
  // Write the ascii data
  for (int i = 0; i < h.width * h.height; i++) {
     fprintf(fh, "%d\n%d\n%d\n", buffer[i].red, buffer[i].green, buffer[i].blue);
  }
}

// Reads P6 data
void readP6(Pixel *buffer, Header h, FILE *fh) {
  // Read RGB triples
  for (int i = 0; i < h.width * h.height; i++) {
     buffer[i].red = fgetc(fh);
     buffer[i].green = fgetc(fh);
     buffer[i].blue = fgetc(fh);
  }
  if (ferror(fh) != 0) {
     fprintf(stderr, "Error: Unable to read data.");
     exit(1);
  }
}

// Writes P6 data
void writeP6(Pixel *buffer, Header h, FILE *fh) {
  // Write header
  fprintf(fh, "P%d\n%d %d\n%d\n", h.magicNumber, h.width, h.height, h.maxColor);
  // Write binary data
  for (int i = 0; i < h.width * h.height; i++) {
     fputc(buffer[i].red, fh);
     fputc(buffer[i].green, fh);
     fputc(buffer[i].blue, fh);
  }
}

// Skips lines that begin with '#'
void skipComments(FILE *fh) {
  char c = fgetc(fh);
  // Skip all comment lines
  while (c == '#') {
    // Go to the end of the line
    do {
      c = fgetc(fh);
      // Comments are potentially not closed
      if (c == EOF) {
        fprintf(stderr, "Error: Reached EOF when parsing comment.");
        exit(1);
      }
    } while (c != '\n');
    
    // Get the next character to test for a comment line
    c = fgetc(fh);
  }
  
  // The standard library does not have a peek so we get and unget instead.
  ungetc(c, fh);
}




