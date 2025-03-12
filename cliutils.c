#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h> 

void print_num_cpus() {
    long num_cpus = sysconf(_SC_NPROCESSORS_ONLN);
    if (num_cpus == -1) {
        perror("Error obteniendo el numero de CPUs");
        exit(EXIT_FAILURE);
    }
    printf("Nuamero de CPUs: %ld\n", num_cpus);
}

int main(int argc, char *argv[]) {
    int aflag = 0;
    int bflag = 0;
    char *cvalue = NULL;
    int opt;

    opterr = 0;

    while ((opt = getopt(argc, argv, "ab:c")) != -1) {
        switch (opt) {
            case 'a':
                aflag = 1;
                break;
            case 'b':
                bflag = atoi(optarg);
                break;
            case 'c':
                print_num_cpus();
                break;
            case '?':
                fprintf(stderr, "Uso: %s -a -b <valor> -c\n", argv[0]);
                return EXIT_FAILURE;
            default:
                abort();
        }
    }

    printf("aflag = %d, bflag = %d\n", aflag, bflag);

    for (int index = optind; index < argc; index++) {
        printf("Argumento no-opci\u00f3n: %s\n", argv[index]);
    }

    return 0;
}
