#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>
#include <signal.h>
#include <string.h>
#include "cpu.h"

volatile sig_atomic_t sigint_count = 0;

void handle_sigint(int sig) {
    sigint_count++;
    if (sigint_count == 1) {
        printf("\n¿Realmente desea terminar el programa? (Y/N): ");
        fflush(stdout);
        char response[2];
        if (fgets(response, sizeof(response), stdin) != NULL) {
            if (tolower(response[0]) == 'y') {
                printf("\nTerminando el programa.\n");
                exit(EXIT_FAILURE);
            } else {
                printf("Continuando...\n");
                sigint_count = 0; // Reset count if user chooses 'N'
            }
        } else {
            printf("\nError leyendo la respuesta. Terminando.\n");
            exit(EXIT_FAILURE);
        }
    } else if (sigint_count >= 2) {
        printf("\nTerminando el programa.\n");
        exit(EXIT_FAILURE);
    }
}

int main(int argc, char **argv) {
    int aflag = 0;
    int bflag = 0;
    char *cvalue = NULL;
    int c;

    // Configurar el manejador de señales para Ctrl+C
    signal(SIGINT, handle_sigint);

    opterr = 0;

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
                printf("Obteniendo número de procesadores...\n");
                for (int i = 0; i < 10; i++) {
                    sleep(1);
                }
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
