#ifndef PROC_STATISTICS_H
#define PROC_STATISTICS_H

#include "cpu_statistics.h"

/**
 * @brief Structure representing data for processor
*/
typedef struct ProcStatistics {
    uint8_t cpus_number;
    CpuStatistics total;
    CpuStatistics* cpus;
} ProcStatistics;

#endif // PROC_STATISTICS_H