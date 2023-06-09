#ifndef PROC_STATISTICS_H
#define PROC_STATISTICS_H

#include <stdint.h> // uint8_t, uint32_t

/**
 * @brief Structure representing data for 1 cpu
*/
typedef struct CpuStatistics {
  uint32_t user;
  uint32_t nice;
  uint32_t system;
  uint32_t idle;
  uint32_t iowait;
  uint32_t irq;
  uint32_t sortirq;
  uint32_t steal;
} CpuStatistics;


/**
 * @brief Structure representing data for processor
*/
typedef struct ProcStatistics {
    uint8_t cpus_number;
    CpuStatistics total;
    CpuStatistics* cpus;
} ProcStatistics;

#endif // PROC_STATISTICS_H