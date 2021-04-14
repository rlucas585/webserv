/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   Utils.cpp                                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: rlucas <marvin@codam.nl>                     +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/03/17 19:05:37 by rlucas        #+#    #+#                 */
/*   Updated: 2021/03/31 20:55:19 by rlucas        ########   odam.nl         */
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

size_t strlen(const char* s) {
    size_t i = 0;
    for (; s[i]; i++) {
    }
    return i;
}

const char* strchr(const char* s, int c) {
    size_t i = 0;

    for (; s[i] != c && s[i]; i++) {
    }
    if (s[i] == c) {
        return s + i;
    } else {
        return 0;
    }
}

char* strchr(char* s, int c) {
    size_t i = 0;

    for (; s[i] != c && s[i]; i++) {
    }
    if (s[i] && s[i] == c) {
        return s + i;
    } else {
        return 0;
    }
}

int strcmp(const char* str1, const char* str2) {
    size_t i = 0;
    for (; str1[i] && str2[i]; i++) {
        if (str1[i] != str2[i])
            return str1[i] - str2[i];
    }
    return str1[i] - str2[i];
}

int strncmp(const char* s1, const char* s2, size_t n) {
    size_t i = 0;
    for (; s1[i] && s2[i] && n > 0; i++, n--) {
        if (s1[i] != s2[i])
            return s1[i] - s2[i];
    }
    if (n == 0) {
        return 0;
    }
    return s1[i] - s2[i];
}

char* strpbrk(char* s, const char* accept) {
    size_t i = 0;
    for (; s[i]; i++) {
        if (strchr(accept, s[i]))
            return (s + i);
    }
    return 0;
}

const char* strpbrk(const char* s, const char* accept) {
    size_t i = 0;
    for (; s[i]; i++) {
        if (strchr(accept, s[i]))
            return (s + i);
    }
    return 0;
}

void* memset(void* b, int c, size_t len) {
    unsigned char* p = reinterpret_cast<unsigned char*>(b);
    for (; len != 0; p++, len--) {
        *p = c;
    }
    return b;
}

void* memcpy(void* dest, const void* src, size_t n) {
    unsigned char* destp = reinterpret_cast<unsigned char*>(dest);
    const unsigned char* srcp = reinterpret_cast<const unsigned char*>(src);
    for (; n > 0; n--, destp++, srcp++)
        *destp = *srcp;
    return dest;
}

int atoi(const char* str) {
    size_t i = 0;
    unsigned long result = 0;
    int sign = 1;
    for (; ((str[i] >= '\t' && str[i] <= '\r') || str[i] == ' '); i++) {
    }
    if (str[i] == '+' || str[i] == '-') {
        if (str[i] == '-') {
            sign = -1;
        }
        i += 1;
    }
    for (; str[i] >= '0' && str[i] <= '9'; i++) {
        result = result * 10 + (str[i] - '0');
    }
    if (sign == 1 && result >= 9223372036854775807) {
        return -1;
    }
    if (sign == -1 && result > 9223372036854775807) {
        return 0;
    }
    return static_cast<int>(result * sign);
}

runtime_error::runtime_error(void) : _msg("Undefined error") {}

runtime_error::~runtime_error(void) throw() {}

runtime_error::runtime_error(const char* msg) : _msg(msg) {}

runtime_error::runtime_error(std::string s) : _msg(s) {}

runtime_error::runtime_error(runtime_error const& other) { *this = other; }

runtime_error& runtime_error::operator=(runtime_error const& rhs) {
    if (this == &rhs) {
        return *this;
    }
    _msg = rhs._msg;
    return *this;
}

const char* runtime_error::what(void) const throw() { return (_msg.c_str()); }
} // namespace Utils
