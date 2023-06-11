#include "../include/reader.h"

#include <stdio.h>      // FILE and operations on it
#include <stdlib.h>     // memory operations
#include <sys/stat.h>   // file status
#include <stdbool.h>    // bool
#include "string.h"     // strok, strstr

typedef struct Reader {
  FILE* file;
} Reader;


Reader* reader_new(char* file_path) {
  Reader* reader;
  FILE* file;

  if(file_path == NULL)
    return NULL;

  reader = malloc(sizeof(*reader));

  if(reader == NULL)
    return NULL;

  file = fopen(file_path, "r");
  if(file == NULL) {
    free(reader);
    return NULL;
  }

  *reader =(Reader){
    .file = file,
  };

  return reader;
}

ResultCode reader_read_latest_statistics(Reader* const reader, ProcStatistics* statistics) {
  bool data_read_successfully = false;
  size_t buffer_size = 1024;
  size_t chars_read = 0;
  char* buffer;
  uint8_t cpus_number = 0;
  size_t cpu_num = 0;
  char* file_content;
  char* line;
  int trash;

  if(reader == NULL)
    return NULL_TARGET_ERROR;

  if(statistics == NULL)
    return NULL_TARGET_ERROR;

  buffer = malloc(buffer_size);
  if(buffer == NULL)
    return ALLOCATION_ERROR;

  while(!data_read_successfully) {
    fseek(reader->file, 0, SEEK_SET);
    chars_read = fread(buffer, sizeof(char), buffer_size, reader->file);
    data_read_successfully = !ferror(reader->file) && feof(reader->file);

    if(!data_read_successfully) {
      free(buffer);

      buffer_size *= 2;
      buffer = malloc(buffer_size);

      if(buffer == NULL) {
        return ALLOCATION_ERROR;
      }
    }
  }
  buffer[chars_read] = '\0';

  file_content = buffer;
  while((file_content = strstr(file_content, "cpu")) != NULL) {
    cpus_number++;
    file_content++;
  }
  cpus_number--; // one "cpu" is total
  
  statistics->cpus_number = cpus_number;
  statistics->cpus = malloc(sizeof(CpuStatistics) * cpus_number);

  if(statistics->cpus == NULL) {
    free(buffer);
    return ALLOCATION_ERROR;
  }

  line = strtok(buffer, "\n");
  while (line != NULL){
      if(strstr(line, "cpu") != NULL) {
          if(cpu_num == 0) { // first "cpu" occurrence is statistic for total
              sscanf(line, "cpu %d %d %d %d %d %d %d %d", &(statistics->total.user), &(statistics->total.nice),
                      &(statistics->total.system), &(statistics->total.idle), &(statistics->total.iowait),
                      &(statistics->total.irq), &(statistics->total.sortirq), &(statistics->total.steal));
          }
          else { // next "cpu" occurrences are statistic for single cpu
              sscanf(line, "cpu%d %d %d %d %d %d %d %d %d", &trash, &(statistics->cpus[cpu_num - 1].user), &(statistics->cpus[cpu_num - 1].nice),
                      &(statistics->cpus[cpu_num - 1].system), &(statistics->cpus[cpu_num - 1].idle), &(statistics->cpus[cpu_num - 1].iowait),
                      &(statistics->cpus[cpu_num - 1].irq), &(statistics->cpus[cpu_num - 1].sortirq), &(statistics->cpus[cpu_num - 1].steal));
          }
          if(cpu_num == cpus_number) {
            break;
          }

          cpu_num++;
      }
      else {
        break;
      }

      line = strtok(NULL, "\n");
  }

  free(buffer);

  return SUCCESS;
}

void reader_delete(Reader* const reader) {
  if(reader == NULL)
    return;

  fclose(reader->file);
  free(reader);
}
