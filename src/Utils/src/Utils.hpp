/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   Utils.hpp                                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: rlucas <marvin@codam.nl>                     +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/03/17 19:05:10 by rlucas        #+#    #+#                 */
/*   Updated: 2021/03/19 18:21:30 by rlucas        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTILS_HPP
#define UTILS_HPP

#include <cstddef>
#include <exception>

namespace Utils {
size_t strlen(const char* s);
const char* strchr(const char* s, int c);
char* strchr(char* s, int c);
int strcmp(const char* str1, const char* str2);
int strncmp(const char* s1, const char* s2, size_t n);
char* strpbrk(char* s, const char* accept);
const char* strpbrk(const char* s, const char* accept);

class runtime_error : public std::exception {
    public:
        runtime_error(const char *msg);
        virtual ~runtime_error(void) throw();
        runtime_error(runtime_error const& other);
        runtime_error &operator=(runtime_error const& rhs);

        virtual const char* what(void) const throw();

    private:
        const char*     _msg;

        runtime_error(void);
};
} // namespace Utils

#endif
