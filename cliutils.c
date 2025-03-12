#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "cpu.h"  // Include the CPU header

int main(int argc, char **argv) {
  int aflag = 0;
  int bflag = 0;
  char *cvalue = NULL;
  int c;

  opterr = 0;

  // Add 'p' to the options string
  while ((c = getopt(argc, argv, "ab:c:p")) != -1) {
    switch (c) {
      case 'a':
        aflag = 1;
        break;
      case 'b':
        bflag = atoi(optarg);
        break;
      case 'c':
        cvalue = optarg;
        break;
      case 'p':
        // Handle -p: Print CPU info and exit
        if (get_cpu_info() != 0) {
          exit(EXIT_FAILURE);
        }
        exit(EXIT_SUCCESS);
        break;
      case '?':
        if (optopt == 'c') {
          fprintf(stderr, "Option -%c requires an argument.\n", optopt);
        } else if (isprint(optopt)) {
          fprintf(stderr, "Unknown option `-%c'.\n", optopt);
        } else {
          fprintf(stderr, "Unknown option character `\\x%x'.\n", optopt);
        }
        return 1;
      default:
        abort();
    }
  }

  printf("aflag = %d, bflag = %d, cvalue = %s\n", aflag, bflag, cvalue);

  for (int index = optind; index < argc; index++) {
    printf("Non-option argument %s\n", argv[index]);
  }

  return 0;
}