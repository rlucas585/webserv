/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   Slice.hpp                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: rlucas <marvin@codam.nl>                     +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/03/26 20:48:56 by rlucas        #+#    #+#                 */
/*   Updated: 2021/05/09 23:34:59 by rlucas        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef STR_HPP
#define STR_HPP

#include <exception>
#include <iostream>
#include <string>
#include <sys/types.h>
#include <vector>

#include "../../Option/src/optional.hpp"

class Slice {
  public:
    class Split {
      public:
        Split(const char* start);
        Split(const char* start, const char* delim);
        Split(Slice const& str);
        Split(Slice const& str, const char* delim);
        ~Split(void);
        Split(Split const& src);
        Split& operator=(Split const& rhs);
        Slice next(void);
        template <typename Container>
        Container collect(void) {
            Container c;
            Slice item;

            while ((item = this->next())) {
                c.push_back(item);
            }
            return c;
        }
        bool is_complete(void) const;

      private:
        const char* _remainder;
        const char* _delimiter;
        ssize_t length_remaining;

        Split(void) {}

        const char* _findFirstNotOf(const char* s, const char* reject);
        bool is_from_slice(void) const;
    };

    class SplitN {
      public:
        SplitN(const char* start, size_t n);
        SplitN(const char* start, const char* delim, size_t n);
        SplitN(Slice const& str, size_t n);
        SplitN(Slice const& str, const char* delim, size_t n);
        ~SplitN(void);
        SplitN(SplitN const& src);
        SplitN& operator=(SplitN const& rhs);
        Slice next(void);
        template <typename Container>
        Container collect(void) {
            Container c;
            Slice item;

            while ((item = this->next())) {
                c.push_back(item);
            }
            return c;
        }
        bool is_complete(void) const;

      private:
        const char* _remainder;
        const char* _delimiter;
        ssize_t length_remaining;
        size_t values_left;

        SplitN(void) {}

        const char* _findFirstNotOf(const char* s, const char* reject);
        bool is_from_slice(void) const;
    };

  public:
    class iterator {
      public:
        iterator(const char* start, size_t len, size_t maxLen);
        ~iterator(void);
        iterator(iterator const& src);
        iterator& operator=(iterator const& rhs);
        iterator& operator++();
        iterator operator++(int);
        iterator& operator--();
        iterator operator--(int);
        bool operator==(const iterator& rhs) const;
        bool operator!=(const iterator& rhs) const;
        const char& operator*(void);

      private:
        const char* _p;
        size_t _len;
        size_t _maxLen;
    };

  public:
    Slice(void);
    Slice(const char* data);

    Slice(std::string const& src);
    Slice& operator=(std::string const& str);
    static Slice newSlice(std::string const& str);
    static Slice newSliceWithLength(std::string const& str, size_t len);
    static Slice newSliceWithOffset(std::string const& str, size_t offset);
    static Slice newSliceWithLengthAndOffset(std::string const& str, size_t len, size_t offset);
    operator std::string() const;
    std::string toSliceing(void) const;

    ~Slice(void);
    Slice(Slice const& src) { *this = src; }
    Slice& operator=(Slice const& rhs);
    Slice& operator=(const char* data);

    static Slice newSlice(const char* data);
    static Slice newSliceWithLength(const char* data, size_t len);
    static Slice newSliceWithOffset(const char* data, size_t offset);
    static Slice newSliceWithOffset(Slice const& src, size_t offset);
    static Slice newSliceWithLengthAndOffset(const char* data, size_t len, size_t offset);

    size_t length(void) const;
    size_t count(char c) const;
    Utils::optional<char> front(void) const;
    Utils::optional<Slice> strchr(char c);
    Slice& trim(const char* reject = " \f\n\r\t\v");
    Utils::optional<Slice> find(Slice needle);
    const char* raw(void) const;
    bool isInitialized(void) const;
    iterator begin(void) const;
    iterator end(void) const;

    Split split(const char* delim = " \f\n\r\t\v") const;
    SplitN splitn(size_t n, const char* delim = " \f\n\r\t\v") const;

    bool operator==(Slice const& rhs) const;
    bool operator!=(Slice const& rhs) const;
    bool operator<(Slice const& rhs) const;
    bool operator<=(Slice const& rhs) const;
    bool operator>(Slice const& rhs) const;
    bool operator>=(Slice const& rhs) const;

    operator bool() const;

  private:
    const char* _data;
    size_t _len;

    bool _throwIfUninitialized(void) const;

    Slice(const char* data, size_t len);
    Slice(const char* data, size_t len, size_t offset);
};

bool operator==(std::string lhs, Slice const& rhs);
bool operator!=(std::string lhs, Slice const& rhs);
bool operator<(std::string lhs, Slice const& rhs);
bool operator<=(std::string lhs, Slice const& rhs);
bool operator>(std::string lhs, Slice const& rhs);
bool operator>=(std::string lhs, Slice const& rhs);

std::ostream& operator<<(std::ostream& o, Slice const& str);

void append_slice_to_vector(std::vector<char>& vec, Slice slice);
void append_slice_to_string(std::string& str, Slice slice);

#endif
