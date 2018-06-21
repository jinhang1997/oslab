#ifndef COMMON_H
#define COMMON_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <pwd.h>

#define panic(format, ...) \
  do { \
    fprintf(stdout, "\033[1;33m[%s,%d,%s] " format "\033[0m\n", \
      __FILE__, __LINE__, __func__, ## __VA_ARGS__); \
  } while (0); \
  fflush(stdout); \
  exit(0);

#ifdef DEBUG

#define log2file(log_file, format, ...) \
  do { \
    fprintf(log_file, "[%s,%d,%s] " format "\n", \
      __FILE__, __LINE__, __func__, ## __VA_ARGS__); \
  } while (0);

#define log(format, ...) \
  do { \
    fprintf(stdout, "\033[1;34m[%s,%d,%s] " format "\033[0m\n", \
      __FILE__, __LINE__, __func__, ## __VA_ARGS__); \
  } while (0); \
  fflush(stdout);
  
#define warning(format, ...) \
  do { \
    fprintf(stdout, "\033[1;33m[%s,%d,%s] " format "\033[0m\n", \
      __FILE__, __LINE__, __func__, ## __VA_ARGS__); \
  } while (0); \
  fflush(stdout);

#endif

#endif
