#ifndef READER_H
#define READER_H

#include <stddef.h> // size_t
#include "proc_statistics.h"
#include "result_code.h"

/**
 * @brief reader structure
*/
typedef struct Reader Reader;

/**
 * @brief Create reader
 * 
 * @param read_interval - time between successive file readings
 * 
 * @return pointer to created Reader
*/
Reader* reader_new(size_t read_interval);

/**
 * @brief Read statistics
 * 
 * @param reader - pointer to reader
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
