/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   Utils.hpp                                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: rlucas <marvin@codam.nl>                     +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/03/17 19:05:10 by rlucas        #+#    #+#                 */
/*   Updated: 2021/03/18 17:37:17 by rlucas        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTILS_HPP
#define UTILS_HPP

#include <cstddef>

namespace Utils {
size_t strlen(const char* s);
const char* strchr(const char* s, int c);
char* strchr(char* s, int c);
int strcmp(const char* str1, const char* str2);
int strncmp(const char* s1, const char* s2, size_t n);
char* strpbrk(char* s, const char* accept);
const char* strpbrk(const char* s, const char* accept);
} // namespace Utils

#endif
