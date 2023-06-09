#ifndef READER_H
#define READER_H

#include <stddef.h> // size_t
#include "proc_statistics.h"
#include "result_code.h"

/**
 * @brief Reader structure
*/
typedef struct Reader Reader;

/**
 * @brief Create reader
 * 
 * @param file_path - path to file to read
 * 
 * @return pointer to created reader
*/
Reader* reader_new(char* file_path);

/**
 * @brief Read statistics
 * 
 * @param reader - pointer to reader
 * @param statistics - poiter to statistics structure to save reader result
 * @return operation result
*/
ResultCode reader_read_latest_statistics(Reader* reader, ProcStatistics* statistics);

/**
 * @brief Destroy reader
 * 
 * @param reader - pointer to reader to delete
*/
void reader_delete(Reader* reader);

#endif // READER_H
