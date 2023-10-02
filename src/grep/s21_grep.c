#include "s21_grep.h"

int main(int argc, char *argv[]) {
  int check = 0;
  Flag options = {0};
  options.pattern = 1;
  options.file = 1;
  char arraypattern[STROKA][SYMBOL] = {0};
  char arrayfiles[STROKA][SYMBOL] = {0};
  if (parser_flag(argc, argv, arraypattern, &options)) {
    if (!options.e && !options.f) {
      no_flags(&options, argv, argc, arraypattern, arrayfiles, &check);
    } else
      yesflags(&options, argv, argc, arrayfiles);
    swap(&options);
    if (options.e || options.i || options.v || options.c || options.l ||
        options.n || options.h || options.s || options.f || options.o) {
      grep(options, arraypattern, arrayfiles);
    } else {
      grep_noflag(&options, arraypattern, arrayfiles);
    }
  }
  return 0;
}

void swap(Flag *options) {
  if (options->v) options->o = 0;
  if (options->c) {
    options->o = 0;
    options->n = 0;
  }
  if (options->l) {
    options->o = 0;
    options->n = 0;
  }
}

void grep(Flag options, char arraypattern[STROKA][SYMBOL],
          char arrayfiles[STROKA][SYMBOL]) {
  int count = 0, count_l = 0, count_n = 0, count_c = 0, count_l_v = 0,
      count_c_l = 0, count_c_l_v = 0;
  FILE *fp;
  int count_c_v = 0;
  for (int j = 0; j < options.file; j++) {
    count_n = 0, count_c = 0, count_l = 0, count_l_v = 0, count_c_l = 0,
    count_c_l_v = 0, count_c_v = 0;
    if ((fp = fopen(arrayfiles[j], "r")) == NULL) {
      if (!options.s) fprintf(stderr, "s21_grep: No such file or directory\n");
    } else {
      char str[1000] = {0};
      int regflag = REG_EXTENDED;
      if (options.i) regflag = REG_ICASE;
      while (fgets(str, 1000, fp) != NULL) {
        count_n++;
        work_with_patterns(&count, count_n, str, options, arraypattern,
                           arrayfiles, regflag, j, &count_c, &count_l,
                           &count_c_l, &count_l_v, &count_c_l_v, &count_c_v);
      }
      if (options.c) {
        options_c(options, arrayfiles, j, count_c_v, count_c_l, count_c_l_v,
                  count_c);
      }
      if (options.l && !options.c && !options.v) {
        if (count_l) {
          printf("%s\n", arrayfiles[j]);
        }
      }
      if (options.l && options.v && !options.c) {
        if (count_l_v) {
          printf("%s\n", arrayfiles[j]);
        }
      }
      fclose(fp);
    }
  }
}

void work_with_patterns(int *count, int count_n, char *str, Flag options,
                        char arraypattern[STROKA][SYMBOL],
                        char arrayfiles[STROKA][SYMBOL], int regflag, int j,
                        int *count_c, int *count_l, int *count_ll,
                        int *count_l_v, int *count_ll_v, int *count_c_v) {
  int offset = 0;
  int len = strlen(str);
  *count = 0;
  if (str[len - 1] == '\n') {
    str[len - 1] = '\0';
  }
  for (int i = 0; i < options.pattern; i++) {
    if (function_chek(options, arraypattern, arrayfiles, i, regflag, str, count,
                      count_n, j, &offset)) {
      continue;
    }
  }
  if (*count) {
    *count_c = *count_c + 1;
  } else
    *count_c_v = *count_c_v + 1;
  if (!options.c && !options.l && !options.o) {
    options_f(options, arrayfiles, j, *count, count_n, str);
  }
  if (*count) {
    *count_l = *count_l + 1;
    *count_ll = 1;
  } else {
    *count_l_v = *count_l_v + 1;
    *count_ll_v = 1;
  }
}

int function_chek(Flag options, char arraypattern[STROKA][SYMBOL],
                  char arrayfiles[STROKA][SYMBOL], int i, int regflag,
                  char *str, int *count, int count_n, int j, int *offset) {
  int o = 0;
  int fail_or_success = 0;
  regex_t regex;
  regmatch_t match;
  if (!options.o) {
    regcomp(&regex, arraypattern[i], regflag);
    fail_or_success = regexec(&regex, str, 0, NULL, 0);
    if (!fail_or_success) {
      *count = 1;
    }
  }
  if (options.o) {
    int flag = 1;
    regcomp(&regex, arraypattern[i], regflag);
    while (!regexec(&regex, str + *offset, 1, &match, 0)) {
      if (options.file >= 2 && !options.h && flag) {
        printf("%s:", arrayfiles[j]);
        flag = 0;
      }
      if (options.n && *offset == 0) printf("%d:", count_n);
      for (int i = *offset + match.rm_so; i < *offset + match.rm_eo; i++) {
        printf("%c", str[i]);
      }
      printf("\n");
      *offset += match.rm_eo;
    }
    o = 1;
  }
  regfree(&regex);
  return o;
}

void options_f(Flag options, char arrayfiles[STROKA][SYMBOL], int j, int count,
               int count_n, char *str) {
  if (count && !options.v) {
    if (options.file >= 2 && !options.h) {
      printf("%s:", arrayfiles[j]);
      if (options.n) printf("%d:", count_n);
      printf("%s\n", str);
    } else {
      if (options.n) printf("%d:", count_n);
      printf("%s\n", str);
    }
  }
  if (!count && options.v) {
    if (options.file >= 2 && !options.h) {
      printf("%s:", arrayfiles[j]);
      if (options.n) printf("%d:", count_n);
      printf("%s\n", str);
    } else {
      if (options.n) printf("%d:", count_n);
      printf("%s\n", str);
    }
  }
}

void options_c(Flag options, char arrayfiles[STROKA][SYMBOL], int j,
               int count_c_v, int count_ll, int count_ll_v, int count_c) {
  if (options.file >= 2 && !options.h) {
    printf("%s:", arrayfiles[j]);
    if (options.v && !options.l) {
      printf("%d\n", count_c_v);
    } else if (options.l && !options.v) {
      printf("%d\n", count_ll);
      if (count_ll) printf("%s\n", arrayfiles[j]);
    } else if (options.l && options.v) {
      printf("%d\n", count_ll_v);
      if (count_ll_v) printf("%s\n", arrayfiles[j]);
    } else
      printf("%d\n", count_c);
  } else {
    if (options.v && !options.l) {
      printf("%d\n", count_c_v);
    } else if (options.l && !options.v) {
      printf("%d\n", count_ll);
      if (count_ll) printf("%s\n", arrayfiles[j]);
    } else if (options.l && options.v) {
      printf("%d\n", count_ll_v);
      if (count_ll_v) {
        printf("%s\n", arrayfiles[j]);
      }
    } else
      printf("%d\n", count_c);
  }
}

void grep_noflag(Flag *options, char arraypattern[STROKA][SYMBOL],
                 char arrayfiles[STROKA][SYMBOL]) {
  regex_t regex;
  int fail_or_success;
  FILE *fp;
  char tr[1000];
  for (int i = 0; i < options->file; i++) {
    if ((fp = fopen(arrayfiles[i], "r")) != NULL) {
      while (fgets(tr, 1000, fp) != NULL) {
        int len = strlen(tr);
        if (tr[len - 1] == '\n') tr[len - 1] = '\0';
        regcomp(&regex, arraypattern[0], REG_EXTENDED);
        fail_or_success = regexec(&regex, tr, 0, NULL, 0);
        regfree(&regex);
        if (fail_or_success == 0) {
          if (options->file >= 2 && !options->h) {
            printf("%s:", arrayfiles[i]);
            printf("%s\n", tr);
          } else {
            printf("%s\n", tr);
          }
        }
      }
      fclose(fp);
    } else
      fprintf(stderr, "s21_grep: No such file or directory\n");
  }
}

int parser_flag(int argc, char *argv[], char arraypattern[STROKA][SYMBOL],
                Flag *options) {
  int error = 1, c, check = 0;
  const struct option long_options[] = {
      {"e", 1, 0, 'e'}, {"i", 0, 0, 'i'}, {"v", 0, 0, 'v'}, {"c", 0, 0, 'c'},
      {"l", 0, 0, 'l'}, {"n", 0, 0, 'n'}, {"h", 0, 0, 'h'}, {"f", 1, 0, 'f'},
      {"s", 0, 0, 's'}, {"o", 0, 0, 'o'}, {0, 0, 0, 0}};
  while ((c = getopt_long(argc, argv, "e:ivclnhsof:", long_options, NULL)) !=
         -1) {
    switch (c) {
      case 'e':
        options->e = 1;
        get_pattern_e(options, arraypattern, &check);
        break;
      case 'i':
        options->i = 1;
        break;
      case 'v':
        options->v = 1;
        break;
      case 'c':
        options->c = 1;
        break;
      case 'l':
        options->l = 1;
        break;
      case 'n':
        options->n = 1;
        break;
      case 'h':
        options->h = 1;
        break;
      case 'f':
        options->f = 1;
        get_pattern_f(options, arraypattern, &check);
        break;
      case 's':
        options->s = 1;
        break;
      case 'o':
        options->o = 1;
        break;
      default:
        options = 0;
        error = 0;
        break;
    }
  }
  return error;
}

void yesflags(Flag *options, char *argv[], int argc,
              char arrayfiles[STROKA][SYMBOL]) {
  int countfiles = 0;
  while (optind < argc) {
    if (countfiles == 1) options->file++;
    strcpy(arrayfiles[options->file - 1], argv[optind]);
    optind++;
    countfiles = 1;
  }
}

void no_flags(Flag *options, char *argv[], int argc,
              char arraypattern[STROKA][SYMBOL],
              char arrayfiles[STROKA][SYMBOL], int *check) {
  int count = 1;
  while (optind < argc) {
    if (count == 1) {
      strcpy(arraypattern[options->pattern - 1], argv[optind]);
      count++;
    } else {
      if (*check) options->file++;
      strcpy(arrayfiles[options->file - 1], argv[optind]);
      *check = *check + 1;
    }
    optind++;
  }
}

void get_pattern_e(Flag *options, char arraypattern[STROKA][SYMBOL],
                   int *check) {
  if (*check) options->pattern++;
  strcpy(arraypattern[options->pattern - 1], optarg);
  *check = 1;
}

void get_pattern_f(Flag *options, char arraypattern[STROKA][SYMBOL],
                   int *check) {
  char c[1000];
  FILE *f = fopen(optarg, "r");
  while (fgets(c, 1000, f) != NULL) {
    if (*check) options->pattern++;
    int i = strlen(c);
    if (c[i - 1] == '\n' && i > 1) {
      c[i - 1] = '\0';
    }
    strcpy(arraypattern[options->pattern - 1], c);
    *check = 1;
  }
  fclose(f);
}
