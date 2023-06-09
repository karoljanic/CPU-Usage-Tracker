#ifndef ANALYSED_PROC_STATISTICS_H
#define ANALYSED_PROC_STATISTICS_H

#include <stdint.h> // uint8_t

typedef struct AnalysedProcStatistics {
    uint8_t cpus_number;
    float total;
    float* cpus;
} AnalysedProcStatistics;

#endif // ANALYSED_PROC_STATISTICS_H