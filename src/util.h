#pragma once

#include <errno.h>
#include <stdlib.h>
#include <assert.h>

#if defined(__GNUC__) || defined(__clang__)
#   define NODISCARD __attribute__((warn_unused_result))
#else
#   define NODISCARD
#endif

/**
 * @brief Parses an address provided by the user in a command
 *
 *        There are few issues that are not yet addressed:
 *        
 *        1) Octal ambiguity - when a user makes a mistake with writing `01000` instead of `0x100` the base of the number 
 *           is in octal base (due to C shenanigans of course 0 is a prefix for octal base such as 0x is a prefix for hex)
 *        2) Parsing doesn't handle "stupid" 0x1 shit type addresses - solution would be to check the values in some range 
 *           OR mapped regions of the target process (/proc/<pid>/maps)
 *
 * @param str pointer to a string representing an address
 * @param out pointer to an output unsigned long address variable
 * @returns 0 on success; -1 otherwise;
 */
NODISCARD static inline int parse_address(const char* str, unsigned long* out) {
    if (str == 0 || *str == '\0') {
        return -1;
    }

    char* end;
    errno = 0;

    const unsigned long address = strtoul(str, &end, 0);
    if (errno == ERANGE) {
        return -1;
    }

    if (end == str) {
        return -1;
    }

    if (*end == '\0') {
        *out = address;
        return 0;
    }

    return -1;
}
