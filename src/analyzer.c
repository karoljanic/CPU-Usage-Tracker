#include <stdlib.h>     // memory operations
#include <stdbool.h>    // bool
#include <stdio.h>      // printf

#include "../include/analyzer.h"

typedef struct Analyzer {
  uint64_t proc_total_prev;
  uint64_t proc_idle_prev;
  uint64_t* cpus_total_prev;
  uint64_t* cpus_idle_prev;
  bool prev_initialized;
} Analyzer;

static float single_analyse(CpuStatistics* statistics, uint64_t* total_prev, uint64_t* idle_prev);


Analyzer* analyzer_new() {
    Analyzer* analyzer = malloc(sizeof(*analyzer));
    if(analyzer == NULL)
        return NULL;

    analyzer->prev_initialized = false;
    analyzer->cpus_total_prev = NULL;
    analyzer->cpus_idle_prev = NULL;

    return analyzer;
}

static float single_analyse(CpuStatistics* statistics, uint64_t* total_prev, uint64_t* idle_prev) {
  uint64_t idle = statistics->idle + statistics->iowait;
  uint64_t non_idle = statistics->user + statistics->nice + statistics->system + statistics->irq + statistics->sortirq + statistics->steal;
  uint64_t total = idle + non_idle - *total_prev;
  uint64_t idled = idle - *idle_prev;

  /*
  printf("\n");
  printf("total_prev: %ld\n", *total_prev);
  printf("idle_prev: %ld\n", *idle_prev);
  printf("idle: %ld\n", idle);
  printf("non_idle: %ld\n", non_idle);
  printf("total: %ld\n", total);
  printf("idled: %ld\n", idled);
  */

  float percentage = 0.0;
  if(total != 0.0)
    percentage = (float)(total - idled) / (float)total * 100.0;

  *total_prev = idle + non_idle;
  *idle_prev = idle;

  return percentage;
}

ResultCode analyzer_analyse_statistics(Analyzer* analyzer, ProcStatistics* row_statistics, AnalysedProcStatistics* analysed_statistics) {
  if(analyzer == NULL)
    return NULL_TARGET_ERROR;
    
  if(row_statistics == NULL)
    return NULL_TARGET_ERROR;

  if(analysed_statistics == NULL)
    return NULL_TARGET_ERROR;

  if(!analyzer->prev_initialized) {
    analyzer->cpus_total_prev = malloc(sizeof(uint64_t) * row_statistics->cpus_number);
    if(analyzer->cpus_total_prev == NULL)
      return ALLOCATION_ERROR;

    analyzer->cpus_idle_prev = malloc(sizeof(uint64_t) * row_statistics->cpus_number);
    if(analyzer->cpus_idle_prev == NULL) {
      free(analyzer->cpus_total_prev);

      return ALLOCATION_ERROR;
    }
    
    uint64_t idle = row_statistics->total.idle + row_statistics->total.iowait;
    uint64_t non_idle = row_statistics->total.user + row_statistics->total.nice + row_statistics->total.system + row_statistics->total.irq + row_statistics->total.sortirq + row_statistics->total.steal;
    
    analyzer->proc_total_prev = idle + non_idle;
    analyzer->proc_idle_prev = idle;
    
    for(uint64_t i = 0; i < row_statistics->cpus_number; i++) {
        idle = row_statistics->cpus[i].idle + row_statistics->cpus[i].iowait;
        non_idle = row_statistics->cpus[i].user + row_statistics->cpus[i].nice + row_statistics->cpus[i].system + row_statistics->cpus[i].irq +row_statistics->cpus[i].sortirq + row_statistics->cpus[i].steal;
    
        analyzer->cpus_total_prev[i] = idle + non_idle;
        analyzer->cpus_idle_prev[i] = idle;
    }

    analyzer->prev_initialized = true;

    analysed_statistics->cpus = NULL;
    analysed_statistics->cpus_number = row_statistics->cpus_number;

    return INITIALISATION_SUCCESS;
  }

  analysed_statistics->cpus = malloc(sizeof(float) * row_statistics->cpus_number);
  if(analysed_statistics->cpus == NULL)
    return ALLOCATION_ERROR;

  analysed_statistics->cpus_number = row_statistics->cpus_number;
  analysed_statistics->total = single_analyse(&row_statistics->total, &analyzer->proc_total_prev, &analyzer->proc_idle_prev);
  for(uint8_t i = 0; i < row_statistics->cpus_number; i++) {
    analysed_statistics->cpus[i] = single_analyse(&(row_statistics->cpus[i]), &(analyzer->cpus_total_prev[i]), &(analyzer->cpus_idle_prev[i]));
  }

  return SUCCESS;
}

void analyzer_delete(Analyzer* analyzer) {
  if(analyzer == NULL)
    return;
  
  if(analyzer->cpus_total_prev != NULL)
    free(analyzer->cpus_total_prev);

  if(analyzer->cpus_idle_prev != NULL)
    free(analyzer->cpus_idle_prev);

  free(analyzer);
}