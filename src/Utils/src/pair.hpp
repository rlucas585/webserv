/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   pair.hpp                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: rlucas <marvin@codam.nl>                     +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/03/27 16:30:44 by rlucas        #+#    #+#                 */
/*   Updated: 2021/03/27 16:31:11 by rlucas        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef PAIR_HPP
#define PAIR_HPP

namespace Utils {
template <typename T1 = int, typename T2 = int> struct pair {
  public:
    pair(void) : first(), second() {}
    pair(const T1& x, const T2& y) : first(x), second(y) {}
    template <class U1, class U2> pair(const pair<U1, U2>& p) : first(p.first), second(p.second) {}
    pair& operator=(const pair& other) {
        first = other.first;
        second = other.second;
        return *this;
    }

  public:
    T1 first;
    T2 second;
};

template <typename T1, typename T2> bool operator==(pair<T1, T2> const& p1, pair<T1, T2> const& p2) {
    return p1.first == p2.first && p1.second == p2.second;
}
template <typename T1, typename T2> bool operator!=(pair<T1, T2> const& p1, pair<T1, T2> const& p2) {
    return !(p1 == p2);
}
template <typename T1, typename T2> bool operator<(pair<T1, T2> const& p1, pair<T1, T2> const& p2) {
    if (!(p1.first == p2.first))
        return false;
    return p1.second < p2.second;
}
template <typename T1, typename T2> bool operator<=(pair<T1, T2> const& p1, pair<T1, T2> const& p2) {
    return !(p2 < p1);
}
template <typename T1, typename T2> bool operator>(pair<T1, T2> const& p1, pair<T1, T2> const& p2) { return p2 < p1; }
template <typename T1, typename T2> bool operator>=(pair<T1, T2> const& p1, pair<T1, T2> const& p2) {
    return !(p1 < p2);
}

template <typename T1, typename T2> pair<T1, T2> make_pair(T1 t, T2 u) { return pair<T1, T2>(t, u); }
} // namespace Utils

#endif
