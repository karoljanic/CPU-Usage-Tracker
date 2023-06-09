#ifndef PRINTER_H
#define PRINTER_H

#include "analysed_proc_statistics.h"

/**
 * @brief Printing analysed processor statistics
 * 
 * @param statistics - pointer to statistics to visualise
*/
void print_proc_usage(AnalysedProcStatistics* statistics);

#endif // PRINTER_H