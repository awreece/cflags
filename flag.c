// Copyright 2013 Alex Reece.

#include <getopt.h>
#include <stdlib.h>
#include <string.h>

#include "flag.h"

typedef struct {
        flag_type_t type;
        void* val;

        const char* name;
        const char* usage;
} flag_t;

static flag_t flags[MAX_FLAGS] = {
  [0] = {f_bool, NULL, "help", "Print this help message"}
};
static size_t nflags = 1;

// help is MAX_FLAGS so we can distinguish it from a standard flag or a bool
// flag.
static struct option options[MAX_FLAGS*2] = {
  [0] = {"help", no_argument, NULL, MAX_FLAGS}
};

static size_t noptions = 1;

inline static char* atoa(const char* input) {
        size_t nbytes = strlen(input) + 1;  // +1 for the null byte.
        char* ret = (char*) malloc(nbytes);
        memcpy(ret, input, nbytes);
        return ret;
}

size_t __add_option(const char* name, int has_arg, int* flag, int val) {
        assert_lt(noptions, MAX_FLAGS*2);

        options[noptions].name = name;
        options[noptions].has_arg = has_arg;
        options[noptions].flag = flag;
        options[noptions].val = val;

        return noptions++;
}

size_t __add_flag(flag_type_t type, void* val,
                  const char* name, const char* usage) {
        assert_lt(nflags, MAX_FLAGS);

        flags[nflags].type = type;
        flags[nflags].val = val;
        flags[nflags].name = name;
        flags[nflags].usage = usage;

        return nflags++;
}

static void parse_flag(flag_t* flag, char* val) {
        switch (flag->type) {
        case f_int:
                *(int*)flag->val = atoi(val);
                break;
        case f_double:
                *(double*)flag->val = atof(val);
                break;
        case f_string:
                *(char**)flag->val = atoa(val);
                break;
        default:
                printf("Forbiddent type %d\n", flag->type);
                exit(1);
        }
}

static void print_flag(const flag_t* flag) {
        switch (flag->type) {
        case f_bool:
                printf("\t--%s/--no%s\n", flag->name, flag->name);
                break;
        case f_int:
                printf("\t--%s=%d\n", flag->name, *(int*)flag->val);
                break;
        case f_double:
                printf("\t--%s=%f\n", flag->name, *(double*)flag->val);
                break;
        case f_string:
                printf("\t--%s=%s\n", flag->name, *(char**)flag->val);
                break;
        default:
                assertf(0, "Unknown type %d", flag->type);
        }

        printf("\t\t%s\n", flag->usage);
}

static void usage(const char* progname) {
        printf("Usage: %s\n", progname);

        size_t findex;
        for (findex = 0; findex < nflags; findex++) {
                print_flag(&flags[findex]);
        }
}

void parse_flags(int argc, char* const* argv) {
        int findex;
        while ((findex = getopt_long(argc, argv, "", options, NULL)) != -1) {
                if (findex == MAX_FLAGS || findex == '?') {
                        usage(argv[0]);
                        exit(0);
                } else if (findex == 0) {
                        // If we successfully parsed a bool flag.
                        continue;
                }

                flag_t* flag = &flags[findex];
                parse_flag(flag, optarg);
        }
}
