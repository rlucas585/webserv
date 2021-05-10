/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   Slice.cpp                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: rlucas <marvin@codam.nl>                     +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/03/26 20:48:54 by rlucas        #+#    #+#                 */
/*   Updated: 2021/05/03 11:36:06 by rlucas        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "Slice.hpp"

#include "../../Utils/src/Utils.hpp"

Slice::Split::Split(const char* start)
    : _remainder(start), _delimiter(" \f\n\r\t\v"), length_remaining(-1) {}

Slice::Split::Split(const char* start, const char* delim)
    : _remainder(start), _delimiter(delim), length_remaining(-1) {}

Slice::Split::Split(Slice const& src)
    : _remainder(src.raw()), _delimiter(" \f\n\r\t\v"), length_remaining(src.length()) {}

Slice::Split::Split(Slice const& src, const char* delim)
    : _remainder(src.raw()), _delimiter(delim), length_remaining(src.length()) {}

Slice::Split::~Split(void) {}

Slice::Split::Split(Split const& src) { *this = src; }

Slice::Split& Slice::Split::operator=(Split const& rhs) {
    if (this == &rhs) {
        return *this;
    }
    _remainder = rhs._remainder;
    _delimiter = rhs._delimiter;
    length_remaining = rhs.length_remaining;
    return *this;
}

Slice Slice::Split::next(void) {
    Slice slice;

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
            slice = Slice::newSliceWithLength(_remainder, length_remaining);
        else
            slice = Slice::newSlice(_remainder);
    } else
        slice = Slice::newSliceWithLength(_remainder, nextDelim - _remainder);
    if (this->is_from_slice()) {
        length_remaining -= (nextDelim - _remainder);
    }
    _remainder = _findFirstNotOf(nextDelim, _delimiter);
    return slice;
}

bool Slice::Split::is_complete(void) const {
    if (this->is_from_slice()) {
        if (length_remaining == 0)
            return true;
    }
    return _remainder == 0;
}

const char* Slice::Split::_findFirstNotOf(const char* s, const char* reject) {
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

bool Slice::Split::is_from_slice(void) const { return length_remaining != -1; }

Slice::SplitN::SplitN(const char* start, size_t n)
    : _remainder(start), _delimiter(" \f\n\r\t\v"), length_remaining(-1), values_left(n) {}

Slice::SplitN::SplitN(const char* start, const char* delim, size_t n)
    : _remainder(start), _delimiter(delim), length_remaining(-1), values_left(n) {}

Slice::SplitN::SplitN(Slice const& src, size_t n)
    : _remainder(src.raw()), _delimiter(" \f\n\r\t\v"), length_remaining(src.length()),
      values_left(n) {}

Slice::SplitN::SplitN(Slice const& src, const char* delim, size_t n)
    : _remainder(src.raw()), _delimiter(delim), length_remaining(src.length()), values_left(n) {}

Slice::SplitN::~SplitN(void) {}

Slice::SplitN::SplitN(SplitN const& src) { *this = src; }

Slice::SplitN& Slice::SplitN::operator=(SplitN const& rhs) {
    if (this == &rhs) {
        return *this;
    }
    _remainder = rhs._remainder;
    _delimiter = rhs._delimiter;
    length_remaining = rhs.length_remaining;
    values_left = rhs.values_left;
    return *this;
}

Slice Slice::SplitN::next(void) {
    Slice slice;

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
            slice = Slice::newSliceWithLength(_remainder, length_remaining);
            length_remaining = 0;
        } else {
            slice = Slice::newSlice(_remainder);
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
            slice = Slice::newSliceWithLength(_remainder, length_remaining);
        else
            slice = Slice::newSlice(_remainder);
    } else
        slice = Slice::newSliceWithLength(_remainder, nextDelim - _remainder);
    if (this->is_from_slice()) {
        length_remaining -= (nextDelim - _remainder);
    }
    _remainder = _findFirstNotOf(nextDelim, _delimiter);
    values_left -= 1;
    return slice;
}

bool Slice::SplitN::is_complete(void) const {
    if (this->is_from_slice()) {
        if (length_remaining == 0)
            return true;
    }
    return _remainder == 0;
}

const char* Slice::SplitN::_findFirstNotOf(const char* s, const char* reject) {
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

bool Slice::SplitN::is_from_slice(void) const { return length_remaining != -1; }

Slice::iterator::iterator(const char* start, size_t len, size_t maxLen)
    : _p(start), _len(len), _maxLen(maxLen) {}

Slice::iterator::~iterator(void) {}

Slice::iterator::iterator(iterator const& src) { *this = src; }

Slice::iterator& Slice::iterator::operator=(iterator const& rhs) {
    if (this == &rhs) {
        return *this;
    }
    _p = rhs._p;
    _len = rhs._len;
    _maxLen = rhs._maxLen;
    return *this;
}

Slice::iterator& Slice::iterator::operator++() {
    if (_len == 0)
        return *this;
    _len -= 1;
    if (_len != 0)
        ++_p;
    return *this;
}

Slice::iterator Slice::iterator::operator++(int) {
    iterator tmp(*this);
    operator++();
    return tmp;
}

Slice::iterator& Slice::iterator::operator--() {
    if (_len == _maxLen) {
        throw Utils::runtime_error("Decrementing iterator at start of Slice slice");
    }
    if (_len != 0)
        --_p;
    _len += 1;
    return *this;
}

Slice::iterator Slice::iterator::operator--(int) {
    iterator tmp(*this);
    operator--();
    return tmp;
}

bool Slice::iterator::operator==(const iterator& rhs) const {
    return _p == rhs._p && _len == rhs._len && _maxLen == rhs._maxLen;
}

bool Slice::iterator::operator!=(const iterator& rhs) const { return !(*this == rhs); }

const char& Slice::iterator::operator*(void) {
    if (_len == 0) {
        throw Utils::runtime_error("Dereferencing iterator past end of Slice");
    }
    return *_p;
}

Slice::Slice(void) : _data(0), _len(0) {}

Slice::Slice(const char* data) : _data(data) { _len = (_data) ? Utils::strlen(_data) : 0; }

Slice::Slice(const char* data, size_t len) : _data(data), _len(len) {}

Slice::Slice(const char* data, size_t len, size_t offset) : _data(data + offset), _len(len) {}

Slice::Slice(std::string const& src) : _data(src.c_str()), _len(src.length()) {}

Slice& Slice::operator=(std::string const& str) {
    _data = str.c_str();
    _len = str.length();
    return *this;
}

Slice Slice::newSlice(std::string const& str) { return Slice(str.c_str()); }

Slice Slice::newSliceWithLength(std::string const& str, size_t len) {
    return Slice(str.c_str(), len);
}

Slice Slice::newSliceWithOffset(std::string const& str, size_t offset) {
    return Slice::newSliceWithOffset(str.c_str(), offset);
}

Slice Slice::newSliceWithLengthAndOffset(std::string const& str, size_t len, size_t offset) {
    return Slice::newSliceWithLengthAndOffset(str.c_str(), len, offset);
}

Slice::operator std::string() const { return std::string(_data, _len); }

std::string Slice::toSliceing(void) const { return std::string(_data, _len); }

bool operator==(std::string lhs, Slice const& rhs) { return rhs == lhs; }
bool operator!=(std::string lhs, Slice const& rhs) { return rhs != lhs; }
bool operator<(std::string lhs, Slice const& rhs) { return rhs > lhs; }
bool operator<=(std::string lhs, Slice const& rhs) { return rhs >= lhs; }
bool operator>(std::string lhs, Slice const& rhs) { return rhs < lhs; }
bool operator>=(std::string lhs, Slice const& rhs) { return rhs <= lhs; }

Slice::~Slice(void) {}

Slice& Slice::operator=(Slice const& rhs) {
    if (this == &rhs) {
        return *this;
    }
    _data = rhs._data;
    _len = rhs._len;
    return *this;
}

Slice& Slice::operator=(const char* data) {
    _data = data;
    _len = (_data) ? Utils::strlen(_data) : 0;
    return *this;
}

Slice Slice::newSlice(const char* data) { return Slice(data); }

Slice Slice::newSliceWithLength(const char* data, size_t len) {
    if (!data)
        throw Utils::runtime_error("Initializing Slice with length, but null pointer");
    if (len > Utils::strlen(data))
        throw Utils::runtime_error("Initializing Slice with length greater than data");
    return Slice(data, len);
}

Slice Slice::newSliceWithOffset(const char* data, size_t offset) {
    size_t len;
    if (!data)
        throw Utils::runtime_error("Initializing Slice with offset, but null pointer");
    if (offset > Utils::strlen(data))
        throw Utils::runtime_error("Initializing Slice with offset greater than length of data");
    len = Utils::strlen(data) - offset;
    return Slice(data, offset, len);
}

Slice Slice::newSliceWithOffset(Slice const& src, size_t offset) {
    if (offset > src._len)
        throw Utils::runtime_error(
            "Initializing Slice with offset greater than length of source Slice");
    return Slice(src._data + offset, src._len - offset);
}

Slice Slice::newSliceWithLengthAndOffset(const char* data, size_t len, size_t offset) {
    if (!data)
        throw Utils::runtime_error("Initializing Slice with offset + length, but null pointer");
    if (offset + len > Utils::strlen(data))
        throw Utils::runtime_error(
            "Initializing Slice with offset + length greater than length of data");
    return Slice(data, len, offset);
}

size_t Slice::length(void) const {
    if (_throwIfUninitialized())
        return 0;
    return _len;
}

size_t Slice::count(char c) const {
    size_t count = 0;

    if (!this->isInitialized())
        return 0;
    for (size_t i = 0; i < _len; i++) {
        if (_data[i] == c)
            count += 1;
    }
    return count;
}

Utils::optional<Slice> Slice::strchr(char c) {
    Utils::optional<Slice> ret = Utils::nullopt;

    for (size_t i = 0; i < _len; i++) {
        if (_data[i] == c) {
            ret.set(Slice::newSliceWithLength(_data + i, _len - i));
            break;
        }
    }
    return ret;
}

Slice& Slice::trim(const char* reject) {
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

const char* Slice::raw(void) const { return _data; }

bool Slice::isInitialized(void) const {
    if (_data)
        return true;
    return false;
}

Slice::iterator Slice::begin(void) const {
    if (_throwIfUninitialized())
        return iterator(_data, 0, 0);
    return iterator(_data, _len, _len);
}

Slice::iterator Slice::end(void) const {
    if (_throwIfUninitialized())
        return iterator(_data, 0, 0);
    if (_len == 0)
        return iterator(_data, 0, 0);
    return iterator(_data + _len - 1, 0, _len);
}

Slice::Split Slice::split(const char* delim) const { return Slice::Split(*this, delim); }

Slice::SplitN Slice::splitn(size_t n, const char* delim) const {
    return Slice::SplitN(*this, delim, n);
}

bool Slice::operator==(Slice const& rhs) const {
    return Utils::strncmp(_data, rhs._data, std::max(_len, rhs._len)) == 0;
}

bool Slice::operator!=(Slice const& rhs) const { return !(*this == rhs); }

bool Slice::operator<(Slice const& rhs) const {
    return Utils::strncmp(_data, rhs._data, std::max(_len, rhs._len)) < 0;
}

bool Slice::operator<=(Slice const& rhs) const { return !(rhs < *this); }

bool Slice::operator>(Slice const& rhs) const { return rhs < *this; }

bool Slice::operator>=(Slice const& rhs) const { return !(*this < rhs); }

Slice::operator bool() const { return this->isInitialized(); }

bool Slice::_throwIfUninitialized(void) const {
    if (!this->isInitialized())
        throw Utils::runtime_error("Operation called using Uninitialized Slice");
    return false;
}

std::ostream& operator<<(std::ostream& o, Slice const& str) {
    if (!str.isInitialized()) {
        o << "null";
        return o;
    }
    for (Slice::iterator it = str.begin(); it != str.end(); it++)
        o << *it;
    return o;
}

// Interactions with other containers.
void append_slice_to_vector(std::vector<char>& vec, Slice slice) {
    for (Slice::iterator it = slice.begin(); it != slice.end(); it++) {
        vec.push_back(*it);
    }
}

void append_slice_to_string(std::string& str, Slice slice) {
    for (Slice::iterator it = slice.begin(); it != slice.end(); it++) {
        str.push_back(*it);
    }
}
