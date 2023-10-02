#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct s_flags {
  int b;
  int e;
  int n;
  int s;
  int t;
  int v;
  int option_index;
} t_flags;

int flags(int argc, char *argv[], char *short_options, t_flags *input);
void input_v(char *symbol);
void input_t(char *symbol);
int flag_s(int *currentline, int *currentline1, int *nextline, char *symbol);

int main(int argc, char *argv[]) {
  t_flags input = {0};
  char *short_options = "+beEnstTv";
  FILE *fp;
  char c[1000] = {0};
  if (flags(argc, argv, short_options, &input)) {
    for (; optind < argc; optind++) {
      strcpy(c, argv[optind]);
      fp = fopen(argv[optind], "r");
      if (fp == NULL) {
        fprintf(stderr, "No such file or directory\n");
      } else {
        char symbol = {0}, lastsymbol = '\n';
        int currentline = 0, currentline1 = 0, nextline = 0, currentline3 = 1;
        while ((symbol = fgetc(fp)) != EOF) {
          if (input.s) {
            if (flag_s(&currentline, &currentline1, &nextline, &symbol))
              continue;
            else
              nextline = currentline;
          }
          if (input.b && ((symbol != '\n') && (lastsymbol == '\n')))
            printf("%6d\t", currentline3++);
          if ((input.n && !(input.b)) && (lastsymbol == '\n'))
            printf("%6d\t", currentline3++);
          if (input.e && symbol == '\n') {
            printf("$");
          }
          if (input.t && symbol == '\t') {
            input_t(&symbol);
          }
          if (input.v) {
            input_v(&symbol);
          }
          lastsymbol = symbol;
          printf("%c", symbol);
        }
        fclose(fp);
      }
    }
  } else
    fprintf(stderr, "usage: cat [-benstuv] [file ...]");
  return 0;
}

int flags(int argc, char *argv[], char *short_options, t_flags *input) {
  struct option long_options[] = {
      {"number-nonblank", no_argument, NULL, 'b'},
      {"number", optional_argument, NULL, 'n'},
      {"squeeze-blank", required_argument, NULL, 's'},
      {NULL, 0, NULL, 0}};
  int res = 1;
  while ((input->option_index = getopt_long(argc, argv, short_options,
                                            long_options, NULL)) != -1) {
    if (input->option_index == 'b')
      input->b++;
    else if (input->option_index == 'E')
      input->e++;
    else if (input->option_index == 'n')
      input->n++;
    else if (input->option_index == 's')
      input->s++;
    else if (input->option_index == 'T')
      input->t++;
    else if (input->option_index == 'v')
      input->v++;
    else if (input->option_index == 'e') {
      input->e++;
      input->v++;
    } else if (input->option_index == 't') {
      input->t++;
      input->v++;
    } else
      res = 0;
  }
  return res;
}

int flag_s(int *currentline, int *currentline1, int *nextline, char *symbol) {
  int rez = 0;
  *currentline = 0;
  if (*symbol == '\n' && *currentline1 == 0) {
    *currentline = 1;
  }
  if (*symbol == '\n' && *currentline1 != 0) {
    *currentline1 = 0;
  }
  if (*symbol != '\n' && *currentline1 == 0) {
    *currentline1 = 1;
  }
  if (*currentline && *nextline) {
    rez = 1;
  }
  return rez;
}

void input_v(char *symbol) {
  if ((*symbol >= 0 && *symbol <= 8) || (*symbol >= 11 && *symbol <= 31)) {
    printf("^");
    *symbol += 64;
  }
  if (*symbol == 127) {
    printf("^");
    *symbol -= 64;
  }
}

void input_t(char *symbol) {
  printf("^");
  *symbol = 'I';
}
