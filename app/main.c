#include <signal.h>     // actions
#include <string.h>     // memset
#include <stdatomic.h>  // atomic variables
#include <stdbool.h>    // true, false
#include <time.h>       // nanosleep
#include <stdio.h>      // printf
#include <stdlib.h>     // memory operations

#include "../include/cpu_tracker.h"
#include "../include/reader.h"
#include "../include/analyzer.h"
#include "../include/printer.h"
#include "../include/buffer.h"

typedef volatile sig_atomic_t flag_type;
#define compare_flag(a, b) a == b

static flag_type termination_flag = ATOMIC_VAR_INIT(0);

static Reader* reader;
static Analyzer* analyzer;
static Printer* printer;
//static Buffer* reader_analyzer_buffer;
//static Buffer* analyzer_printer_buffer;

// Watchdog flag to make sure only one watchdog can execute exit() function which is not thread-safe
//static atomic_flag g_wd_flag = ATOMIC_FLAG_INIT;

/*
static void signal_handler(int signum) {
  if(signum == SIGTERM)
    termination_flag = 1;
}

static void* reader_func() {
    //WDCommunication * wdc = (WDCommunication *) args;
    while(true) {
        ProcStatistics stats;
        reader_read_latest_statistics(reader, &stats);
        if(buffer_pop(reader_analyzer_buffer, &stats, 2) != SUCCESS) {
            //logger_write("Reader error while adding data to the buffer", LOG_ERROR);
            pthread_exit(NULL);
        }
        //logger_write("READER - new data to analyze sent", LOG_INFO);

        if(compare_flag(termination_flag, 1))
            break;

        //logger_write("READER - goes to sleep", LOG_INFO);
        //watchdog_send_signal(wdc);

        struct timespec sleepTime;
        sleepTime.tv_sec = 1;
        sleepTime.tv_nsec = 0;
        nanosleep(&sleepTime, NULL);
    }

    pthread_exit(NULL);
}


static void* analyzer_func() {
    //WDCommunication* wdc = (WDCommunication *) args;
    ProcStatistics* data = malloc(sizeof(Stats)*(g_no_cpus+1));

    bool first_iter = true;

    uint64_t* prev_total = calloc(g_no_cpus+1 ,sizeof(uint64_t));
    uint64_t* prev_idle = calloc(g_no_cpus+1 ,sizeof(uint64_t));

    if(prev_total == NULL || prev_idle == NULL)
    {
        logger_write("Allocation error", LOG_ERROR);
        // One of the pointers is possibly not NULL, free(ptr) - If ptr is NULL, no operation is performed.
        free(prev_idle);
        free(prev_total);
        pthread_exit(NULL);
    }
    while(compare_flag(termination_flag, 0))
    {
        // Pop from buffer
        // Queue structure is thread safe
        if (queue_dequeue(g_reader_analyzer_queue, data, 2) != QSUCCESS)
        {
            logger_write("Analyzer error while removing data from the buffer", LOG_ERROR);
            break;
        }
        logger_write("ANALYZER - new data to analyze received", LOG_INFO);

        // Consume / Analyze
        if (first_iter)
        {
            analyzer_update_prev(prev_total, prev_idle, *data, g_no_cpus);
            first_iter = false;
        }
        else
        {
            UsagePercentage to_print;
            to_print.cores_pr = malloc(sizeof(double)*(g_no_cpus));
            //Total
            to_print.total_pr =  analyzer_analyze(&prev_total[0], &prev_idle[0], data->total);

            // Cores
            for (size_t j = 0; j < g_no_cpus; ++j)
                to_print.cores_pr[j] =  analyzer_analyze(&prev_total[j+1], &prev_idle[j+1], data->cpus[j]);

            // Send to print
            if(queue_enqueue(g_analyzer_printer_queue, &to_print, 2) != QSUCCESS)
            {
                logger_write("Analyzer error while adding data to the buffer", LOG_ERROR);
                break;
            }
            logger_write("ANALYZER - new data to print sent", LOG_INFO);
        }
        free(data->cpus);
        watchdog_send_signal(wdc);
    }
    // Cleanup
    free(data);
    free(prev_total);
    free(prev_idle);
    pthread_exit(NULL);
}

static void* printer_func(void* args)
{
    WDCommunication * wdc = (WDCommunication *) args;
    // system func - there should not be any problems related to thread safety as long as there are no other threads attempting to call system concurrently.
    system("clear");
    // printf("\t\t\033[3;33m*** CUT - CPU Usage Tracker ~ Sebastian Wozniak ***\033[0m\n");  // print here using tput
    UsagePercentage* to_print = malloc(sizeof(double)*(g_no_cpus+1));
    if(to_print == NULL)
    {
        logger_write("Allocation error in printer thread", LOG_ERROR);
        pthread_exit(NULL);
    }
    while(compare_flag(termination_flag, 0))
    {
        size_t i;
        // Remove from buffer
        if (queue_dequeue(g_analyzer_printer_queue, to_print, 2) != QSUCCESS)
        {
            logger_write("Printer error while removing data from the buffer", LOG_ERROR);
            break;
        }
        logger_write("PRINTER - new data to print received", LOG_INFO);

        // Print
        // system("tput cup 1 0");  // - Better than clear, but it's buggy when terminal window is too small
        system("clear");
        printf("\t\t\033[3;33m*** CUT - CPU Usage Tracker ~ Sebastian Wozniak ***\033[0m\n"); // print here using clear
        printf("TOTAL:\t ╠");
        size_t pr = (size_t) to_print->total_pr;
        for (i = 0; i < pr; i++)
            printf("▒");

        for (i = 0; i < 100 - pr; i++)
            printf("-");

        printf("╣ %.1f%% \n", to_print->total_pr);

        for (size_t j = 0; j < g_no_cpus; j++)
        {
            printf("\033[0;%zumcpu%zu:\t ╠", 31 + (j % 6), j+1);
            pr = (size_t) to_print->cores_pr[j];
            for (i = 0; i < pr; i++)
                printf("▒");

            for (i = 0; i < 100 - pr; i++)
                printf("-");

            printf("╣ %.1f%% \n", to_print->cores_pr[j]);
        }
        printf("\033[0m");
        free(to_print->cores_pr);

        watchdog_send_signal(wdc);
    }
    free(to_print);
    pthread_exit(NULL);
}

static void cleanup(void)
{
    CPURawStats to_free_1;
    while(!queue_is_empty(g_reader_analyzer_queue))
    {
        queue_dequeue(g_reader_analyzer_queue, &to_free_1, 2);
        free(to_free_1.cpus);
    }
    UsagePercentage to_free_2;
    while(!queue_is_empty(g_analyzer_printer_queue))
    {
        queue_dequeue(g_analyzer_printer_queue,&to_free_2, 2);
        free(to_free_2.cores_pr);
    }
    queue_delete(g_reader_analyzer_queue);
    queue_delete(g_analyzer_printer_queue);
}

static void thread_join_create_error(const char* msg) {
    //logger_write(msg, LOG_ERROR);
    cleanup();
    //logger_destroy();
}
*/

int main(void) {
  reader = reader_new("/proc/stat");
  analyzer = analyzer_new();
  printer = printer_new("#");

  ProcStatistics stats;
  AnalysedProcStatistics analysed;

  reader_read_latest_statistics(reader, &stats);
  printf("%d\n", analyzer_analyse_statistics(analyzer, &stats, &analysed));

  free(stats.cpus);
  free(analysed.cpus);

  for(int i = 0; i < 2; i++) {
    struct timespec sleepTime;
    sleepTime.tv_sec = 1;
    sleepTime.tv_nsec = 0;
    nanosleep(&sleepTime, NULL);

    reader_read_latest_statistics(reader, &stats);
    printf("%d\n", analyzer_analyse_statistics(analyzer, &stats, &analysed));

    printer_print_proc_usage(printer, &analysed);

    free(stats.cpus);
    free(analysed.cpus);
  }

  reader_delete(reader);
  analyzer_delete(analyzer);
  printer_delete(printer);

  return 0;
}
