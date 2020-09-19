#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define TRUE 1
#define FALSE 0
#define BAD_NUMBER_ARGS 1

/**
 * Parses the command line.
 *
 * argc: the number of items on the command line (and length of the
 *       argv array) including the executable
 * argv: the array of arguments as strings (char* array)
 * bits: the integer value is set to TRUE if bits output indicated
 *       outherwise FALSE for hex output
 *
 * returns the input file pointer (FILE*)
 **/
FILE* parseCommandLine(int argc, char** argv, int* bits) {
  if (argc > 2) {
    printf("Usage: %s [-b|-bits]\n", argv[0]);
    exit(BAD_NUMBER_ARGS);
  }

  if (argc == 2 &&
      (strcmp(argv[1], "-b") == 0 || strcmp(argv[1], "-bits") == 0)) {
    *bits = TRUE;
  } else {
    *bits = FALSE;
  }

  return stdin;
}


/**
 * Writes data to stdout in hexadecimal.
 *
 * See myxxd.md for details.
 *
 * data: an array of no more than 16 characters
 * size: the size of the array
 **/
void printDataAsHex(unsigned char* data, size_t size) {
  printf("TODO 1: printDataAsHex (2)");
}

/**
 * Writes data to stdout as characters.
 *
 * See myxxd.md for details.
 *
 * data: an array of no more than 16 characters
 * size: the size of the array
 **/
void printDataAsChars(unsigned char* data, size_t size) {
  for (size_t i = 0; i < size; i++) {
    printf("%c", data[i]);
  }
}

void readAndPrintInputAsHex(FILE* input) {
  unsigned char data[16];
  int numBytesRead = fread(data, 1, 16, input);
  unsigned int offset = 0;

  while (numBytesRead != 0) {
    printf("%08x:", offset);
    offset += numBytesRead;
    printDataAsHex(data, numBytesRead);
    printf("  ");
    printDataAsChars(data, numBytesRead);
    printf("\n");
    numBytesRead = fread(data, 1, 16, input);
  }
}

int* calcBits(unsigned char* data) {
  printf("Entering calcBits\n");
  int target = (intptr_t) data - '0';
  int* out[8];

  for(size_t i = 0; i < 8; i++) {
    printf("for loop: %lu\n", i);

    if (target % 2 == 1) {
      out[i] = (int*) 1;
      printf("out[%lu] = 1\n", i);
    } else {
      out[i] = (int*) 0;
      printf("out[%lu] = 0\n", i);
    }
    printf("out[%lu] = %d\n", i, out[i]);
    target = target / 2;
  }

  printf("Leaving calcBits\n");
  return *out;
}

void printDataAsBits(unsigned char* data, int numBytesRead) {
  printf("Entering printDataAsBits\n");

  for (size_t i = 0; i < 8; i++) {
    int* out = calcBits((unsigned char*) data[i]);

    if (out == NULL) printf("out = NULL\n");

    for (size_t j = 0; j < 8; j++) {
      printf("%d", out[j]);
    }
  }
}

/**
 * Bits output for xxd.
 *
 * See myxxd.md for details.
 *
 * input: input stream
 **/
void readAndPrintInputAsBits(FILE *input) {
  printf("Entering readAndPrintInputAsBits\n");
  unsigned char data[8];
  int numBytesRead = fread(data, 1, 8, input);
  unsigned int offset = 0;

  while (numBytesRead != 0) {
    printf("%02x:", offset);
    offset += numBytesRead;
    printDataAsBits(data, numBytesRead);
    printf(" ");
    printDataAsChars(data, numBytesRead);
    printf("\n");
    numBytesRead = fread(data, 1, 8, input);
  }
}

int main(int argc, char **argv) {
  int bits = FALSE;
  FILE *input = parseCommandLine(argc, argv, &bits);

  if (bits == FALSE) {
    readAndPrintInputAsHex(input);
  } else {
    readAndPrintInputAsBits(input);
  }
  return 0;
}
