// Copyright 2013 Alex Reece.

#ifndef FLAG_H_
#define FLAG_H_

#include <getopt.h>
#include <stdint.h>
#include <string.h>

#define MAX_FLAGS 30

typedef enum {
        f_bool,
        f_int,
        f_double,
        f_string
} flag_type_t;

// Parses the flags, mutating argv so eventually all non-flag arguments are at
// the end.
void parse_flags(int argc, char *const *argv);

// TODO(awreece) Don't pollute global namespace?
typedef char* string;
typedef int bool;
#define true 1
#define false 0

size_t __add_option(const char* name, int has_arg,
                                  int* flag, int val);
size_t __add_flag(flag_type_t type, void* val,
                                const char* name, const char* usage);

#define FLAG_TYPE(type, name, default, usage)                               \
type name = default;                                                        \
__attribute__((constructor)) void add_flag_ ## name() {                     \
        size_t findex = __add_flag(f_##type, &name, #name, usage);          \
        __add_option(#name, required_argument, NULL, findex);               \
}

#define FLAG_BOOL(name, default, usage)                                     \
bool name = default;                                                        \
__attribute__((constructor)) void add_flag_ ## name() {                     \
        __add_flag(f_bool, &name, #name, usage);                            \
        __add_option(#name, no_argument, &name, 1);                         \
        __add_option("no" #name, no_argument, &name, 0);                    \
}

#define FLAG_INT(name, default, usage) FLAG_TYPE(int, name, default, usage)

#define FLAG_DOUBLE(name, default, usage) \
  FLAG_TYPE(double, name, default, usage)

#define FLAG_STRING(name, default, usage) \
  FLAG_TYPE(string, name, default, usage)

#endif  // FLAG_H_
