#ifndef C3_SIMPLEBASHUTILS_0_SRC_GREP_S21_GREP_H_
#define C3_SIMPLEBASHUTILS_0_SRC_GREP_S21_GREP_H_

#include <getopt.h>
#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define STROKA 200
#define SYMBOL 600

typedef struct {
  int e;
  int i;
  int v;
  int c;
  int l;
  int n;
  int h;
  int s;
  int f;
  int o;
  int pattern;
  int file;
} Flag;

int parser_flag(int argc, char *argv[], char arraypattern[STROKA][SYMBOL],
                Flag *options);
void get_pattern_e(Flag *options, char arraypattern[STROKA][SYMBOL],
                   int *check);
void get_pattern_f(Flag *options, char arraypattern[STROKA][SYMBOL],
                   int *check);
void yesflags(Flag *options, char *argv[], int argc,
              char arrayfiles[STROKA][SYMBOL]);
void no_flags(Flag *options, char *argv[], int argc,
              char arraypattern[STROKA][SYMBOL],
              char arrayfiles[STROKA][SYMBOL], int *check);
void grep_noflag(Flag *options, char arraypattern[STROKA][SYMBOL],
                 char arrayfiles[STROKA][SYMBOL]);
void grep(Flag options, char arraypattern[STROKA][SYMBOL],
          char arrayfiles[STROKA][SYMBOL]);
void swap(Flag *options);
void options_c(Flag options, char arrayfiles[STROKA][SYMBOL], int j,
               int count_c_v, int count_ll, int count_ll_v, int count_c);
void options_f(Flag options, char arrayfiles[STROKA][SYMBOL], int j, int count,
               int count_n, char *str);
int function_chek(Flag options, char arraypattern[STROKA][SYMBOL],
                  char arrayfiles[STROKA][SYMBOL], int i, int regflag,
                  char *str, int *count, int count_n, int j, int *offset);
void work_with_patterns(int *count, int count_n, char *str, Flag options,
                        char arraypattern[STROKA][SYMBOL],
                        char arrayfiles[STROKA][SYMBOL], int regflag, int j,
                        int *count_c, int *count_l, int *count_ll,
                        int *count_l_v, int *count_ll_v, int *count_c_v);

#endif