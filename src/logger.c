#include <stdio.h>      // FILE and operations on it
#include <stdlib.h>     // memory operations

#include "../include/logger.h"

typedef struct Logger {
    FILE* file;
} Logger;

Logger* logger_new(char* logs_file_path) {
  Logger* logger;
  FILE* file;

  if(logs_file_path == NULL)
    return NULL;

  logger= malloc(sizeof(Logger));
  
  if(logger == NULL)
    return NULL;

  file = fopen(logs_file_path, "a+");
  
  if(file == NULL) {
    free(logger);
    return NULL;
  }

  *logger = (Logger){
    .file = file
  };

  return logger;
}

void logger_log(Logger* logger, LogTag tag, const char* message) {
    if(tag == LogTagDebug)
        fprintf(logger->file, "%s", "[D]    ");
    else if(tag == LogTagInfo)
        fprintf(logger->file, "%s", "[I]    ");
    else if(tag == LogTagError)
        fprintf(logger->file, "%s", "[E]    ");
    else
        fprintf(logger->file, "%s", "[ ]    ");

    fprintf(logger->file, "%s\n", message);
    fflush(logger->file);
}

void logger_delete(Logger* logger) {
  if (logger == NULL)
    return;
  
  if(logger->file != NULL)
    fclose(logger->file);

  free(logger);
}
