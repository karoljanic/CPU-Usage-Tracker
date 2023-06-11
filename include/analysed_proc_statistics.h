#ifndef ANALYSED_PROC_STATISTICS_H
#define ANALYSED_PROC_STATISTICS_H

#include <stdint.h> // uint8_t

typedef struct AnalysedProcStatistics {
  float* cpus;
  float total;
  uint8_t cpus_number;
  char padding[3];
} AnalysedProcStatistics;

#endif // ANALYSED_PROC_STATISTICS_H
