#include <stdio.h>      // printf
#include <stdlib.h>     // memory operations
#include "../include/printer.h"

typedef struct Printer {
    char* marker;
} Printer;

static void visualise_cpu_usage(float percentage, const char* marker);

void visualise_cpu_usage(float percentage, const char* marker) {
    int progress = (int)(percentage / 10.0f);
    
    printf("[");

    for(int i = 0; i < progress; i++)
        printf("%s", marker);

    for(int i = progress; i < 10; i++)
        printf(" ");

    printf("] %0.2f%%", (double)percentage);
}

Printer* printer_new(char* progress_marker) {
  Printer* printer = malloc(sizeof(*printer));

  if(printer == NULL)
    return NULL;

  *printer =(Printer){
    .marker = progress_marker,
  };

  return printer;
}

void printer_print_proc_usage(Printer* printer, AnalysedProcStatistics* statistics) {
    if(printer == NULL)
        return;

    if(statistics == NULL)
        return;

    printf("\033[H\033[J");     // clear screen
    printf("***  Super CPU Tracker  ***\n");

    printf("total: ");
    visualise_cpu_usage(statistics->total, printer->marker);
    printf("\n");
    
    for(uint8_t i = 0; i < statistics->cpus_number; i++) {
        printf("cpu %d: ", i);
        visualise_cpu_usage(statistics->cpus[i], printer->marker);
        printf("\n");
    }

    printf("\n");
}

void printer_delete(Printer* printer) {
    if(printer == NULL)
        return;

    free(printer);
}
