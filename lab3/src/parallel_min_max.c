#include <ctype.h>
#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <sys/time.h>
#include <sys/types.h>
#include <sys/wait.h>

#include <getopt.h>

#include "find_min_max.h"
#include "utils.h"

#define MIN(A, B) (((A) > (B)) ? (B) : (A))

int main(int argc, char **argv) {
  int seed = -1;
  int array_size = -1;
  int pnum = -1;
  bool with_files = false;

  while (true) {
    int current_optind = optind ? optind : 1;

    static struct option options[] = {{"seed", required_argument, 0, 0},
                                      {"array_size", required_argument, 0, 0},
                                      {"pnum", required_argument, 0, 0},
                                      {"by_files", no_argument, 0, 'f'},
                                      {0, 0, 0, 0}};

    int option_index = 0;
    int c = getopt_long(argc, argv, "f", options, &option_index);

    if (c == -1) break;

    switch (c) {
      case 0:
        switch (option_index) {
          case 0:
            seed = atoi(optarg);
            if (seed <= 0) {
              printf("seed is a positive number\n");
              return 1;
            }
            break;
          case 1:
            array_size = atoi(optarg);
            if (array_size <= 0) {
              printf("array_size is a positive number\n");
              return 1;
            }
            break;
          case 2:
            pnum = atoi(optarg);
            if (pnum <= 0) {
              printf("pnum is a positive number\n");
              return 1;
            }
            break;
          case 3:
            with_files = true;
            break;

          defalut:
            printf("Index %d is out of options\n", option_index);
        }
        break;
      case 'f':
        with_files = true;
        break;

      case '?':
        break;

      default:
        printf("getopt returned character code 0%o?\n", c);
    }
  }

  if (optind < argc) {
    printf("Has at least one no option argument\n");
    return 1;
  }

  if (seed == -1 || array_size == -1 || pnum == -1) {
    printf("Usage: %s --seed \"num\" --array_size \"num\" --pnum \"num\" \n",
           argv[0]);
    return 1;
  }

  int *array = malloc(sizeof(int) * array_size);
  if (array == NULL)
    {
      printf("Malloc error!");
      return 1;
    }
  GenerateArray(array, array_size, seed);
  //for (int i = 0; i < array_size; i++) printf("%d ", array[i]);
  //printf("\n");
  int active_child_processes = 0;

  struct timeval start_time;
  gettimeofday(&start_time, NULL);

  int part_size = array_size / pnum;
  
  char *fn;
  int len = 0;
  int pfds[2];

  if (with_files) 
  { 
    int num = pnum;
    while (num) {num = num / 10; len++;}
    fn = malloc(sizeof(char) * (20 + len));
    if (fn == NULL)
    {
      printf("Malloc error!");
      return 1;
    }
  }
  else
  {
    if (pipe(pfds) == -1) 
    {
      printf("Pipe opening error!");
      return 1;
    }
  }
  for (int i = 0; i < pnum; i++) {
    pid_t child_pid = fork();
    if (child_pid >= 0) {
      // successful fork
      active_child_processes += 1;
      if (child_pid == 0) {
        struct MinMax min_max = GetMinMax(array, i * part_size, MIN((i + 1) * part_size, array_size));
        if (with_files) {
          FILE* fp;
          snprintf(fn, 20 + len, "parallel_data/part_%d", i);
          fp = fopen(fn, "wb");
          if (fp == NULL) {
            printf("File %s opening error!", fn);
            return 1;
          }
          fwrite(&min_max, sizeof(min_max), 1, fp);
          if (!fclose(fp))
          {
            printf("File %s closing error!", fn);
            return 1;
          }
        } else {
          close(pfds[0]);
          write(pfds[1], &min_max, sizeof(min_max));
          close(pfds[1]);
        }
        return 0;
      }

    } else {
      printf("Fork failed!\n");
      return 1;
    }
  }
  close(pfds[1]);

  while (active_child_processes > 0) {
    wait(NULL);
    active_child_processes -= 1;
  }

  struct MinMax min_max;
  min_max.min = INT_MAX;
  min_max.max = INT_MIN;

  for (int i = 0; i < pnum; i++) {
    struct MinMax cur_min_max;
    cur_min_max.min = INT_MAX;
    cur_min_max.max = INT_MIN;

    if (with_files) {
      FILE* fp;
      snprintf(fn, 20 + len, "parallel_data/part_%d", i);
      fp = fopen(fn, "rb");
      if (fp == NULL) {
        printf("File %s opening error!", fn);
        return 1;
      }
      fread(&cur_min_max, sizeof(min_max), 1, fp);
      if (!fclose(fp))
      {
        printf("File %s closing error!", fn);
        return 1;
      }

    } else {
      read(pfds[0], &cur_min_max, sizeof(cur_min_max));
    }

    if (cur_min_max.min < min_max.min) min_max.min = cur_min_max.min;
    if (cur_min_max.max > min_max.max) min_max.max = cur_min_max.max;
  }

  if (!with_files) close(pfds[0]); 

  struct timeval finish_time;
  gettimeofday(&finish_time, NULL);

  double elapsed_time = (finish_time.tv_sec - start_time.tv_sec) * 1000.0;
  elapsed_time += (finish_time.tv_usec - start_time.tv_usec) / 1000.0;

  free(array);

  printf("Min: %d\n", min_max.min);
  printf("Max: %d\n", min_max.max);
  printf("Elapsed time: %fms\n", elapsed_time);
  fflush(NULL);
  return 0;
}
