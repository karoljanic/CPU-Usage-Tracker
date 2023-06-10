#ifndef PRINTER_H
#define PRINTER_H

#include "analysed_proc_statistics.h"

/**
 * @brief Printer structure
*/
typedef struct Printer Printer;

/**
 * @brief Create printer
 * 
 * @param progress_marker - marker to draw progress
 * 
 * @return pointer to created printer
*/
Printer* printer_new(char* progress_marker);

/**
 * @brief Printing analysed processor statistics
 * 
 * @param printer - pointer to printer
 * @param statistics - pointer to statistics to visualise
*/
void printer_print_proc_usage(Printer* printer, AnalysedProcStatistics* statistics);

/**
 * @brief Destroy printer
 * 
 * @param reader - pointer to printer to delete
*/
void printer_delete(Printer* printer);

#endif // PRINTER_H