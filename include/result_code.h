#ifndef RESULT_CODE_H
#define RESULT_CODE_H

/**
 * @brief Enum representing function result
*/
typedef enum {
    SUCCESS,
    NULL_TARGET_ERROR,
    ALLOCATION_ERROR,
    TIMEOUT_ERROR
} ResultCode;

#endif // RESULT_CODE_H