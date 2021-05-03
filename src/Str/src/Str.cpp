/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   Str.cpp                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: rlucas <marvin@codam.nl>                     +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/03/26 20:48:54 by rlucas        #+#    #+#                 */
/*   Updated: 2021/05/03 11:36:06 by rlucas        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "Str.hpp"

#include "../../Utils/src/Utils.hpp"

Str::Split::Split(const char* start)
    : _remainder(start), _delimiter(" \f\n\r\t\v"), length_remaining(-1) {}

Str::Split::Split(const char* start, const char* delim)
    : _remainder(start), _delimiter(delim), length_remaining(-1) {}

Str::Split::Split(Str const& src)
    : _remainder(src.raw()), _delimiter(" \f\n\r\t\v"), length_remaining(src.length()) {}

Str::Split::Split(Str const& src, const char* delim)
    : _remainder(src.raw()), _delimiter(delim), length_remaining(src.length()) {}

Str::Split::~Split(void) {}

Str::Split::Split(Split const& src) { *this = src; }

Str::Split& Str::Split::operator=(Split const& rhs) {
    if (this == &rhs) {
        return *this;
    }
    _remainder = rhs._remainder;
    _delimiter = rhs._delimiter;
    length_remaining = rhs.length_remaining;
    return *this;
}

Str Str::Split::next(void) {
    Str slice;

    if (this->is_from_slice()) {
        if (length_remaining == 0)
            return slice;
    }
    if (_remainder == NULL)
        return slice;

    const char* nextDelim;
    if (this->is_from_slice())
        nextDelim = Utils::strpbrklen(_remainder, _delimiter, length_remaining);
    else
        nextDelim = Utils::strpbrk(_remainder, _delimiter);

    if (nextDelim == NULL) {
        if (this->is_from_slice())
            slice = Str::newSliceWithLength(_remainder, length_remaining);
        else
            slice = Str::newSlice(_remainder);
    } else
        slice = Str::newSliceWithLength(_remainder, nextDelim - _remainder);
    if (this->is_from_slice()) {
        length_remaining -= (nextDelim - _remainder);
    }
    _remainder = _findFirstNotOf(nextDelim, _delimiter);
    return slice;
}

bool Str::Split::is_complete(void) const {
    if (this->is_from_slice()) {
        if (length_remaining == 0)
            return true;
    }
    return _remainder == 0;
}

const char* Str::Split::_findFirstNotOf(const char* s, const char* reject) {
    size_t i = 0;

    if (s == 0)
        return 0;
    for (; s[i]; s++) {
        if (!Utils::strchr(reject, s[i]))
            break;
        if (length_remaining != -1) {
            length_remaining -= 1;
            if (length_remaining == 0)
                break;
        }
    }
    if (s[i] == '\0')
        return 0;
    if (length_remaining != -1) {
        if (length_remaining == 0)
            return 0;
    }
    return s;
}

bool Str::Split::is_from_slice(void) const { return length_remaining != -1; }

Str::SplitN::SplitN(const char* start, size_t n)
    : _remainder(start), _delimiter(" \f\n\r\t\v"), length_remaining(-1), values_left(n) {}

Str::SplitN::SplitN(const char* start, const char* delim, size_t n)
    : _remainder(start), _delimiter(delim), length_remaining(-1), values_left(n) {}

Str::SplitN::SplitN(Str const& src, size_t n)
    : _remainder(src.raw()), _delimiter(" \f\n\r\t\v"), length_remaining(src.length()),
      values_left(n) {}

Str::SplitN::SplitN(Str const& src, const char* delim, size_t n)
    : _remainder(src.raw()), _delimiter(delim), length_remaining(src.length()), values_left(n) {}

Str::SplitN::~SplitN(void) {}

Str::SplitN::SplitN(SplitN const& src) { *this = src; }

Str::SplitN& Str::SplitN::operator=(SplitN const& rhs) {
    if (this == &rhs) {
        return *this;
    }
    _remainder = rhs._remainder;
    _delimiter = rhs._delimiter;
    length_remaining = rhs.length_remaining;
    values_left = rhs.values_left;
    return *this;
}

Str Str::SplitN::next(void) {
    Str slice;

    if (this->is_from_slice()) {
        if (length_remaining == 0 || values_left == 0)
            return slice;
    }
    if (_remainder == NULL || values_left == 0)
        return slice;

    // Additional logic for SplitN to return everything remaining if values_left == 1
    if (values_left == 1) {
        values_left = 0;
        if (this->is_from_slice()) {
            slice = Str::newSliceWithLength(_remainder, length_remaining);
            length_remaining = 0;
        } else {
            slice = Str::newSlice(_remainder);
        }
        _remainder = 0;
        return slice;
    }

    const char* nextDelim;
    if (this->is_from_slice())
        nextDelim = Utils::strpbrklen(_remainder, _delimiter, length_remaining);
    else
        nextDelim = Utils::strpbrk(_remainder, _delimiter);

    if (nextDelim == NULL) {
        if (this->is_from_slice())
            slice = Str::newSliceWithLength(_remainder, length_remaining);
        else
            slice = Str::newSlice(_remainder);
    } else
        slice = Str::newSliceWithLength(_remainder, nextDelim - _remainder);
    if (this->is_from_slice()) {
        length_remaining -= (nextDelim - _remainder);
    }
    _remainder = _findFirstNotOf(nextDelim, _delimiter);
    values_left -= 1;
    return slice;
}

bool Str::SplitN::is_complete(void) const {
    if (this->is_from_slice()) {
        if (length_remaining == 0)
            return true;
    }
    return _remainder == 0;
}

const char* Str::SplitN::_findFirstNotOf(const char* s, const char* reject) {
    size_t i = 0;

    if (s == 0)
        return 0;
    for (; s[i]; s++) {
        if (!Utils::strchr(reject, s[i]))
            break;
        if (this->is_from_slice()) {
            length_remaining -= 1;
            if (length_remaining == 0)
                break;
        }
    }
    if (s[i] == '\0')
        return 0;
    if (this->is_from_slice()) {
        if (length_remaining == 0)
            return 0;
    }
    return s;
}

bool Str::SplitN::is_from_slice(void) const { return length_remaining != -1; }

Str::iterator::iterator(const char* start, size_t len, size_t maxLen)
    : _p(start), _len(len), _maxLen(maxLen) {}

Str::iterator::~iterator(void) {}

Str::iterator::iterator(iterator const& src) { *this = src; }

Str::iterator& Str::iterator::operator=(iterator const& rhs) {
    if (this == &rhs) {
        return *this;
    }
    _p = rhs._p;
    _len = rhs._len;
    _maxLen = rhs._maxLen;
    return *this;
}

Str::iterator& Str::iterator::operator++() {
    if (_len == 0)
        return *this;
    _len -= 1;
    if (_len != 0)
        ++_p;
    return *this;
}

Str::iterator Str::iterator::operator++(int) {
    iterator tmp(*this);
    operator++();
    return tmp;
}

Str::iterator& Str::iterator::operator--() {
    if (_len == _maxLen) {
        throw Utils::runtime_error("Decrementing iterator at start of Str slice");
    }
    if (_len != 0)
        --_p;
    _len += 1;
    return *this;
}

Str::iterator Str::iterator::operator--(int) {
    iterator tmp(*this);
    operator--();
    return tmp;
}

bool Str::iterator::operator==(const iterator& rhs) const {
    return _p == rhs._p && _len == rhs._len && _maxLen == rhs._maxLen;
}

bool Str::iterator::operator!=(const iterator& rhs) const { return !(*this == rhs); }

const char& Str::iterator::operator*(void) {
    if (_len == 0) {
        throw Utils::runtime_error("Dereferencing iterator past end of Str");
    }
    return *_p;
}

Str::Str(void) : _data(0), _len(0) {}

Str::Str(const char* data) : _data(data) { _len = (_data) ? Utils::strlen(_data) : 0; }

Str::Str(const char* data, size_t len) : _data(data), _len(len) {}

Str::Str(const char* data, size_t len, size_t offset) : _data(data + offset), _len(len) {}

Str::Str(std::string const& src) : _data(src.c_str()), _len(src.length()) {}

Str& Str::operator=(std::string const& str) {
    _data = str.c_str();
    _len = str.length();
    return *this;
}

Str Str::newSlice(std::string const& str) { return Str(str.c_str()); }

Str Str::newSliceWithLength(std::string const& str, size_t len) { return Str(str.c_str(), len); }

Str Str::newSliceWithOffset(std::string const& str, size_t offset) {
    return Str::newSliceWithOffset(str.c_str(), offset);
}

Str Str::newSliceWithLengthAndOffset(std::string const& str, size_t len, size_t offset) {
    return Str::newSliceWithLengthAndOffset(str.c_str(), len, offset);
}

Str::operator std::string() const { return std::string(_data, _len); }

std::string Str::toString(void) const { return std::string(_data, _len); }

bool operator==(std::string lhs, Str const& rhs) { return rhs == lhs; }
bool operator!=(std::string lhs, Str const& rhs) { return rhs != lhs; }
bool operator<(std::string lhs, Str const& rhs) { return rhs > lhs; }
bool operator<=(std::string lhs, Str const& rhs) { return rhs >= lhs; }
bool operator>(std::string lhs, Str const& rhs) { return rhs < lhs; }
bool operator>=(std::string lhs, Str const& rhs) { return rhs <= lhs; }

Str::~Str(void) {}

Str& Str::operator=(Str const& rhs) {
    if (this == &rhs) {
        return *this;
    }
    _data = rhs._data;
    _len = rhs._len;
    return *this;
}

Str& Str::operator=(const char* data) {
    _data = data;
    _len = (_data) ? Utils::strlen(_data) : 0;
    return *this;
}

Str Str::newSlice(const char* data) { return Str(data); }

Str Str::newSliceWithLength(const char* data, size_t len) {
    if (!data)
        throw Utils::runtime_error("Initializing Str with length, but null pointer");
    if (len > Utils::strlen(data))
        throw Utils::runtime_error("Initializing Str with length greater than data");
    return Str(data, len);
}

Str Str::newSliceWithOffset(const char* data, size_t offset) {
    size_t len;
    if (!data)
        throw Utils::runtime_error("Initializing Str with offset, but null pointer");
    if (offset > Utils::strlen(data))
        throw Utils::runtime_error("Initializing Str with offset greater than length of data");
    len = Utils::strlen(data) - offset;
    return Str(data, offset, len);
}

Str Str::newSliceWithOffset(Str const& src, size_t offset) {
    if (offset > src._len)
        throw Utils::runtime_error(
            "Initializing Str with offset greater than length of source Str");
    return Str(src._data + offset, src._len - offset);
}

Str Str::newSliceWithLengthAndOffset(const char* data, size_t len, size_t offset) {
    if (!data)
        throw Utils::runtime_error("Initializing Str with offset + length, but null pointer");
    if (offset + len > Utils::strlen(data))
        throw Utils::runtime_error(
            "Initializing Str with offset + length greater than length of data");
    return Str(data, len, offset);
}

size_t Str::length(void) const {
    if (_throwIfUninitialized())
        return 0;
    return _len;
}

size_t Str::count(char c) const {
    size_t count = 0;

    if (!this->isInitialized())
        return 0;
    for (size_t i = 0; i < _len; i++) {
        if (_data[i] == c)
            count += 1;
    }
    return count;
}

Utils::optional<Str> Str::strchr(char c) {
    Utils::optional<Str> ret = Utils::nullopt;

    for (size_t i = 0; i < _len; i++) {
        if (_data[i] == c) {
            ret.set(Str::newSliceWithLength(_data + i, _len - i));
            break;
        }
    }
    return ret;
}

Str& Str::trim(const char* reject) {
    // While _data contains a char in reject, move it forward
    while (Utils::strchr(reject, *_data) && _len > 0) {
        _data += 1;
        _len -= 1;
    }
    if (_len == 0)
        return *this;
    while (Utils::strchr(reject, *(_data + _len - 1)) && _len > 0) {
        _len -= 1;
    }
    return *this;
}

const char* Str::raw(void) const { return _data; }

bool Str::isInitialized(void) const {
    if (_data)
        return true;
    return false;
}

Str::iterator Str::begin(void) const {
    if (_throwIfUninitialized())
        return iterator(_data, 0, 0);
    return iterator(_data, _len, _len);
}

Str::iterator Str::end(void) const {
    if (_throwIfUninitialized())
        return iterator(_data, 0, 0);
    if (_len == 0)
        return iterator(_data, 0, 0);
    return iterator(_data + _len - 1, 0, _len);
}

Str::Split Str::split(const char* delim) const { return Str::Split(*this, delim); }

Str::SplitN Str::splitn(size_t n, const char* delim) const { return Str::SplitN(*this, delim, n); }

bool Str::operator==(Str const& rhs) const {
    return Utils::strncmp(_data, rhs._data, std::max(_len, rhs._len)) == 0;
}

bool Str::operator!=(Str const& rhs) const { return !(*this == rhs); }

bool Str::operator<(Str const& rhs) const {
    return Utils::strncmp(_data, rhs._data, std::max(_len, rhs._len)) < 0;
}

bool Str::operator<=(Str const& rhs) const { return !(rhs < *this); }

bool Str::operator>(Str const& rhs) const { return rhs < *this; }

bool Str::operator>=(Str const& rhs) const { return !(*this < rhs); }

Str::operator bool() const { return this->isInitialized(); }

bool Str::_throwIfUninitialized(void) const {
    if (!this->isInitialized())
        throw Utils::runtime_error("Operation called using Uninitialized Str");
    return false;
}

std::ostream& operator<<(std::ostream& o, Str const& str) {
    if (!str.isInitialized()) {
        o << "null";
        return o;
    }
    for (Str::iterator it = str.begin(); it != str.end(); it++)
        o << *it;
    return o;
}

// Interactions with other containers.
void append_slice_to_vector(std::vector<char>& vec, Str slice) {
    for (Str::iterator it = slice.begin(); it != slice.end(); it++) {
        vec.push_back(*it);
    }
}

void append_slice_to_string(std::string& str, Str slice) {
    for (Str::iterator it = slice.begin(); it != slice.end(); it++) {
        str.push_back(*it);
    }
}
