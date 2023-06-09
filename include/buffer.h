#ifndef BUFFER_C
#define BUFFER_C

#include <stdbool.h>    // bool
#include <stdint.h>     // uint8_t
#include <stddef.h>     // size_t
#include "result_code.h"

/**
 * @brief Thread safety buffer
*/
typedef struct Buffer Buffer;

/**
 * @brief Create buffer
 * 
 * @param data_size - size of stored data
 * @param buffer_capacity - maximal buffer capacity
 * 
 * @return pointer to created buffer
*/
Buffer* buffer_new(size_t data_size, size_t buffer_capacity);

/**
 * @brief Check if buffer is empty
 * 
 * @param buffer - pointer to buffer
 * 
 * @return true is buffer is empty, false otherwise
*/
bool buffer_is_empty(Buffer* buffer);

/**
 * @brief Check if buffer is full
 * 
 * @param buffer - pointer to buffer
 * 
 * @return true is buffer is full, false otherwise
*/
bool buffer_is_full(Buffer* buffer);

/**
 * @brief Insert element into buffer
 * 
 * @param buffer - pointer to buffer to insert element
 * @param element - pointer to element to insert
 * @param max_push_time - maximum operation time
 * 
 * @return operation result
*/
ResultCode buffer_push(Buffer* buffer, void* element, uint8_t max_push_time);

/**
 * @brief Extract element from buffer
 * 
 * @param buffer - pointer to buffer to extract element
 * @param element - pointer to data to save extracted elemnt
 * @param max_pop_time - maximum operation time
 * 
 * @return operation result
*/
ResultCode buffer_pop(Buffer* buffer, void* element, uint8_t max_pop_time);

/**
 * @brief Destroy buffer
 * 
 * @param buffer - pointer to buffer to delete
*/
void buffer_delete(Buffer* buffer);

#endif // BUFFER_C