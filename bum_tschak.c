#include "not_enough_cli/not_enough_cli.h"

#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static char *fout_name = NULL;

static FILE *fout = NULL;
static FILE *fin = NULL;

static struct option OPTIONS_LONG[] = {{"help", no_argument, NULL, 'h'},
                                       {"version", no_argument, NULL, 'v'},
                                       {"output", required_argument, NULL, 'o'},
                                       {NULL, 0, NULL, 0}};

static char *OPTIONS_HELP[] = {
    ['h'] = "print this help list",
    ['v'] = "print program version",
    ['o'] = "set output file name",
};

_Noreturn static void version() {
  printf("reglex 1.0\n");
  exit(EXIT_SUCCESS);
}

_Noreturn static void usage(int status) {
  FILE *fout = status == 0 ? stdout : stderr;
  nac_print_usage_header(fout, "[OPTION]... [FILE]");
  // TODO: make description more accurate (is LR grammar accurate enough?)
  fprintf(fout, "Generates a parser from a c-like specification file "
                "containing an LR-attributed grammar.\n\n");
  fprintf(fout, "With no FILE, or when FILE is -, read standard input.\n\n");
  nac_print_options(fout);
  exit(status);
}

static void handle_option(char opt) {
  switch (opt) {
  case 'o':
    fout_name = nac_optarg_trimmed();
    if (fout_name[0] == '\0') {
      nac_missing_arg('o');
    }
    break;
  }
}

static void parse_args(int *argc, char ***argv) {
  nac_set_opts(**argv, OPTIONS_LONG, OPTIONS_HELP);
  nac_simple_parse_args(argc, argv, handle_option);

  nac_opt_check_excl("hv");
  nac_opt_check_max_once("hvo");

  if (nac_get_opt('h')) {
    usage(*argc > 0 ? EXIT_FAILURE : EXIT_SUCCESS);
  }
  if (nac_get_opt('v')) {
    if (*argc > 0) {
      usage(EXIT_FAILURE);
    }
    version();
  }

  if (fout_name == NULL) {
    fout = stdout;
  } else {
    fout = fopen(fout_name, "w");
    if (fout == NULL) {
      errx(EXIT_FAILURE, "Failed to open specified output file \"%s\"\n",
           fout_name);
    }
  }

  if (*argc == 0) {
    fin = stdin;
  } else if (*argc == 1) {
    if (strcmp(**argv, "-") == 0) {
      fin = stdin;
    } else {
      fin = fopen(**argv, "r");
    }
  } else {
    usage(EXIT_FAILURE);
  }

  nac_cleanup();
}

int main(int argc, char *argv[]) {
  parse_args(&argc, &argv);
  return EXIT_SUCCESS;
}
