#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <stdbool.h>
#include <time.h>
#include <string.h>
#include "cpu.h"

volatile sig_atomic_t interrupt_count = 0;
volatile sig_atomic_t waiting_confirmation = 0;

void handle_signal(int sig) {
    if (waiting_confirmation) {
        printf("\nSecond interrupt detected. Exiting immediately.\n");
        exit(EXIT_SUCCESS);
    }
    interrupt_count++;
}

void restore_signal_handler() {
    struct sigaction sa;
    sa.sa_handler = handle_signal;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sigaction(SIGINT, &sa, NULL);
}

int main(int argc, char **argv) {
    int aflag = 0;
    int bflag = 0;
    char *cvalue = NULL;
    int c;

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
            case 'p': {
                struct sigaction sa;
                sa.sa_handler = handle_signal;
                sigemptyset(&sa.sa_mask);
                sa.sa_flags = 0;
                sigaction(SIGINT, &sa, NULL);

                printf("Getting CPU information... (press Ctrl+C twice to exit)\n");
                time_t start = time(NULL);
                bool confirmed = false;

                while (time(NULL) - start < 6) {
                    int remaining = 6 - (time(NULL) - start);
                    printf("\rTime remaining: %d seconds ", remaining);
                    fflush(stdout);

                    if (interrupt_count > 0) {
                        waiting_confirmation = 1;
                        printf("\nInterrupt received. Exit? (y/n): ");
                        fflush(stdout);

                        char response[10];
                        if (fgets(response, sizeof(response), stdin)) {
                            if (strcmp(response, "y\n") == 0 || strcmp(response, "Y\n") == 0) {
                                printf("Goodbye!\n");
                                exit(EXIT_SUCCESS);
                            } else if (strcmp(response, "n\n") == 0 || strcmp(response, "N\n") == 0) {
                                start = time(NULL);
                                interrupt_count = 0;
                                waiting_confirmation = 0;
                                printf("Resuming...\n");
                                restore_signal_handler();
                                continue;
                            }
                        }
                        waiting_confirmation = 0;
                    }
                    
                    // Handle 1-second sleep with interrupt checking
                    time_t before_sleep = time(NULL);
                    while (time(NULL) - before_sleep < 1) {
                        if (interrupt_count > 0) break;
                        sleep(1);
                    }
                }

                printf("\n");  // New line after countdown completes
                if (get_cpu_info() != 0) {
                    exit(EXIT_FAILURE);
                }
                exit(EXIT_SUCCESS);
                break;
            }
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