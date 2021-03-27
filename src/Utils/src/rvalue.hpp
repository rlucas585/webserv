/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   rvalue.hpp                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: rlucas <marvin@codam.nl>                     +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/03/27 14:18:38 by rlucas        #+#    #+#                 */
/*   Updated: 2021/03/27 14:19:49 by rlucas        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef RVALUE_HPP
#define RVALUE_HPP

namespace Utils {
// A type that pretends to be an r-value reference
template <typename T>
    class rvalue {
        public:
            explicit rvalue(T& new_ref) : ref(&new_ref) {}

            T& get() const { return *ref; }

            operator T&() const { return *ref; }

        private:
            T* ref;
    };

// returns something that pretends to be an R-value reference
template <typename T>
    rvalue<T> move(T& v) { return rvalue<T>(v); }
}

#endif
