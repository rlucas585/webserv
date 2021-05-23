/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   Utils.cpp                                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: rlucas <marvin@codam.nl>                     +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/03/17 19:05:37 by rlucas        #+#    #+#                 */
/*   Updated: 2021/05/15 13:54:36 by rlucas        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "Utils.hpp"
#include "Slice.hpp"

// Uncomment to show src files are compiled with std=c++98
// #include <array>

#include <iostream>

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

char* strpbrklen(char* s, const char* accept, size_t len) {
    size_t i = 0;
    for (; s[i] && len != 0; i++, len--) {
        if (strchr(accept, s[i]))
            return (s + i);
    }
    return 0;
}

const char* strpbrklen(const char* s, const char* accept, size_t len) {
    size_t i = 0;
    for (; s[i] && len != 0; i++, len--) {
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

long atol(const char* str) {
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
    return static_cast<long>(result * sign);
}

long atol_length(const char* str, size_t length) {
    size_t i = 0;
    unsigned long result = 0;
    int sign = 1;
    for (; ((str[i] >= '\t' && str[i] <= '\r') || str[i] == ' ') && length > 0; i++, length--) {
    }
    if (length == 0)
        return static_cast<long>(result);
    if (str[i] == '+' || str[i] == '-') {
        if (str[i] == '-') {
            sign = -1;
        }
        i += 1;
    }
    for (; str[i] >= '0' && str[i] <= '9' && length > 0; i++, length--) {
        result = result * 10 + (str[i] - '0');
    }
    return static_cast<long>(result * sign);
}

std::string to_string(size_t val) {
    std::string output;
    size_t len = 0;
    for (size_t copy = val; copy > 0; copy /= 10)
        len += 1;
    if (val == 0)
        return std::string("0");
    for (; val > 0; val /= 10)
        output.insert(0, 1, static_cast<char>((val % 10) + '0'));
    return output;
}

bool isspace(char c) { return Utils::strchr(" \f\n\r\t\v", c) != 0; }

bool isnum(char c) { return (c >= '0' && c <= '9'); }

// runtime_error class

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

// Time class

Time::Time(time_t time_val) : inner(time_val) {}

Time::~Time(void) {}

Time::Time(Time const& src) : inner(src.inner) {}

Time& Time::operator=(Time const& rhs) {
    if (&rhs == this) {
        return *this;
    }
    inner = rhs.inner;
    return *this;
}

Time Time::now(void) {
    time_t timer;

    ::time(&timer);
    return Time(timer);
}

std::string Time::to_string(void) {
    struct tm* timeinfo;
    std::string output;

    Utils::memset(buf, '\0', 26);

    timeinfo = localtime(&inner);
    asctime_r(timeinfo, buf);
    std::cout << buf << std::endl;
    for (size_t i = 0; i < 26; i++) {
        if (buf[i])
            output += buf[i];
    }
    return output;
}

std::string Time::to_http_string(void) {
    to_string();
    Slice original = Slice::newSlice(buf);

    Slice::Split iter = original.split();
    std::string output;

    Slice day_name = iter.next();
    Slice month = iter.next();
    Slice day_num = iter.next();
    Slice time = iter.next();
    Slice year = iter.next();

    output.append(day_name)
        .append(", ")
        .append(day_num)
        .append(" ")
        .append(month)
        .append(" ")
        .append(year)
        .append(" ")
        .append(time)
        .append(" ")
        .append("GMT+2");

    return output;
}

} // namespace Utils
