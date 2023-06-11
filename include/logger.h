#ifndef LOGGER_H
#define LOGGER_H

/**
 * @brief Enum representing log tag
*/
typedef enum {
    LogTagDebug,
    LogTagInfo,
    LogTagError
} LogTag;

/**
 * @brief Logger structure
*/
typedef struct Logger Logger;

/**
 * @brief Create logger
 * 
 * @return pointer to created logger
*/
Logger* logger_new(char* logs_file_path);

/**
 * @brief Printing analysed processor statistics
 * 
 * @param tag - tag of message to log
 * @param message - message to log
*/
void logger_log(Logger* logger, LogTag tag, const char* message);

/**
 * @brief Destroy logger
 * 
 * @param logger - pointer to logger to delete
*/
void logger_delete(Logger* logger);

#endif // LOGGER_H
