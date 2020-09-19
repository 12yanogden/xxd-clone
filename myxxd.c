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
 * Writes data to stdout as characters.
 *
 * See myxxd.md for details.
 *
 * data: an array of no more than 16 characters
 * size: the size of the array
 **/
void printDataAsChars(unsigned char* data, size_t size) {
  for (size_t i = 0; i < size; i++) {
    char out = data[i];

    if ((int) out == 10) {
      out = '.';
    }

    printf("%c", out);
  }
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
  for (size_t i = 0; i < 16; i++) {
    // printf("for: %lu\n", i);
    int target = (int) data[i];

    if (target == 0) {
      for (size_t j = 0; j < 2; j++) {
        printf(" ");
      }
    } else {
      printf("%02x", target);

      if (i % 2 == 1) {
        printf(" ");
      }
    }
  }
}

/**
 * Hex output for xxd.
 *
 * See myxxd.md for details.
 *
 * input: input stream
 **/
void readAndPrintInputAsHex(FILE* input) {
  unsigned char data[16];
  int numBytesRead = fread(data, 1, 16, input);
  unsigned int offset = 0;

  while (numBytesRead != 0) {
    printf("%08x: ", offset);
    offset += numBytesRead;
    printDataAsHex(data, numBytesRead);
    printf("  ");
    printDataAsChars(data, numBytesRead);
    printf("\n");
    numBytesRead = fread(data, 1, 16, input);
  }
}

/**
 * Writes data to stdout in bits.
 *
 * See myxxd.md for details.
 *
 * data: an array of no more than 8 characters
 * size: the size of the array
 **/
void printDataAsBits(unsigned char* data, int numBytesRead) {
  for (size_t i = 0; i < 6; i++) {
    int target = (int) data[i];

    if (target == 0) {
      for (size_t j = 0; j < 8; j++) {
        printf(" ");
      }
    } else {
      int out[8];

      for(int j = 7; j >= 0; j--) {
        if (target % 2 == 1) {
          out[j] = 1;
        } else {
          out[j] = 0;
        }

        target /= 2;
      }

      for (size_t j = 0; j < 8; j++) {
        printf("%d", out[j]);
      }

      printf(" ");
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
  unsigned char data[6];
  int numBytesRead = fread(data, 1, 6, input);
  unsigned int offset = 0;

  while (numBytesRead != 0) {
    printf("%08x: ", offset);
    offset += numBytesRead;
    printDataAsBits(data, numBytesRead);
    printf(" ");
    printDataAsChars(data, numBytesRead);
    printf("\n");
    numBytesRead = fread(data, 1, 6, input);
  }
}

int main(int argc, char **argv) {
  printf("Started\n");
  int bits = FALSE;
  FILE *input = parseCommandLine(argc, argv, &bits);

  if (bits == FALSE) {
    readAndPrintInputAsHex(input);
  } else {
    readAndPrintInputAsBits(input);
  }
  return 0;
}
