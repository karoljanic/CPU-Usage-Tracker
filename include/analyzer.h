#ifndef ANALYZER_H
#define ANALYZER_H

#include "proc_statistics.h"
#include "analysed_proc_statistics.h"
#include "result_code.h"

/**
 * @brief Analyzer structure
*/
typedef struct Analyzer Analyzer;

/**
 * @brief Create analyzer
 *
 * @return pointer to created analyzer
*/
Analyzer* analyzer_new(void);

/**
 * @brief Analyze statistics
 * 
 * @param analyzer - pointer to analyzer
 * @param row_statistics - poiter to row statistics structure
 * @param analysed_statistics - pointer to analysed statistics structure
 * @return operation result
*/
ResultCode analyzer_analyse_statistics(Analyzer* analyzer, ProcStatistics* row_statistics, AnalysedProcStatistics* analysed_statistics);

/**
 * @brief Destroy analyzer
 * 
 * @param analyzer - pointer to analyzer to delete
*/
void analyzer_delete(Analyzer* analyzer);

#endif // ANALYZER_H
