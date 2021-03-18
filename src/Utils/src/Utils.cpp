/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   Utils.cpp                                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: rlucas <marvin@codam.nl>                     +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/03/17 19:05:37 by rlucas        #+#    #+#                 */
/*   Updated: 2021/03/18 17:31:32 by rlucas        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "Utils.hpp"

// Uncomment to show src files are compiled with std=c++98
// #include <array>

namespace Utils {
// Uncomment to show src files are compiled with std=c++98
// void failMe(void) {
//  std::array<int, 5>      arr = {1, 2, 3, 4, 5};
//  (void)arr;
// }

size_t      strlen(const char *s) {
    size_t  i = 0;
    for (; s[i]; i++) {}
    return i;
}

const char  *strchr(const char *s, int c) {
    size_t  i = 0;

    for (; s[i] != c && s[i]; i++) {}
    if (s[i] == c) { return s + i; }
    else { return 0; }
}

char        *strchr(char *s, int c) {
    size_t  i = 0;

    for (; s[i] != c && s[i]; i++) {}
    if (s[i] && s[i] == c) { return s + i; }
    else { return 0; }
}

int         strcmp(const char *str1, const char *str2) {
    size_t      i = 0;
    for (; str1[i] && str2[i]; i++) {
        if (str1[i] != str2[i])
            return str1[i] - str2[i];
    }
    return str1[i] - str2[i];
}

int         strncmp(const char *s1, const char *s2, size_t n) {
    size_t      i = 0;
    if (n == 0) { return 0; }
    for (; s1[i] && s2[i] && n > 0; i++, n--) {
        if (s1[i] != s2[i])
            return s1[i] - s2[i];
    }
    return s1[i] - s2[i];
}

char        *strpbrk(char *s, const char *accept) {
    size_t      i = 0;
    for (; s[i]; i++) {
        if (strchr(accept, s[i]))
            return (s + i);
    }
    return 0;
}

const char  *strpbrk(const char *s, const char *accept) {
    size_t      i = 0;
    for (; s[i]; i++) {
        if (strchr(accept, s[i]))
            return (s + i);
    }
    return 0;
}
}
