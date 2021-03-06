#ifndef UTILS_HPP
#define UTILS_HPP

#include <cstddef>
#include <ctime>
#include <exception>
#include <string>

namespace Utils {
size_t strlen(const char* s);
const char* strchr(const char* s, int c);
char* strchr(char* s, int c);
int strcmp(const char* str1, const char* str2);
int strncmp(const char* s1, const char* s2, size_t n);
char* strpbrk(char* s, const char* accept);
const char* strpbrk(const char* s, const char* accept);
char* strpbrklen(char* s, const char* accept, size_t len);
const char* strpbrklen(const char* s, const char* accept, size_t len);
void* memset(void* b, int c, size_t len);
void* memcpy(void* dest, const void* src, size_t n);
int atoi(const char* str);
long atol(const char* str);
long atol_length(const char* str, size_t length);
std::string to_string(size_t val);
bool isspace(char c);
bool isnum(char c);
template <typename T>
T const& min(T const& lhs, T const& rhs) {
    return (rhs < lhs) ? rhs : lhs;
}
template <typename T>
T const& max(T const& lhs, T const& rhs) {
    return (lhs < rhs) ? rhs : lhs;
}

class runtime_error : public std::exception {
  public:
    runtime_error(const char* msg);
    runtime_error(std::string s);
    virtual ~runtime_error(void) throw();
    runtime_error(runtime_error const& other);
    runtime_error& operator=(runtime_error const& rhs);

    virtual const char* what(void) const throw();

  private:
    std::string _msg;

    runtime_error(void);
};

template <typename T>
class unique_ptr {
  public:
    typedef T* pointer;
    typedef T& reference;

    unique_ptr(void) : _p(0) {}
    ~unique_ptr(void) {
        if (_p != 0) {
            delete _p;
        }
    }
    unique_ptr(pointer p) : _p(p) {}
    unique_ptr(const unique_ptr& other) : _p(other._p) {
        // "move" semantics
        unique_ptr& mut_other = const_cast<unique_ptr&>(other);
        mut_other._p = 0;
    }
    unique_ptr& operator=(const unique_ptr& rhs) {
        if (_p != 0) {
            delete _p;
        }
        // "move" semantics
        unique_ptr& mut_rhs = const_cast<unique_ptr&>(rhs);
        _p = rhs._p;
        mut_rhs._p = 0;
        return *this;
    }

    pointer release(void) {
        pointer ret = _p;
        _p = 0;
        return ret;
    }

    pointer get(void) const { return _p; }

    void reset(pointer p = pointer()) {
        if (_p != 0) {
            delete _p;
        }
        _p = p;
    }

    void swap(unique_ptr& other) {
        pointer tmp = _p;

        _p = other._p;
        other._p = tmp;
    }

    reference operator*() { return *_p; }

    pointer operator->() { return _p; }

    bool operator==(const unique_ptr& other) const {
        return ((!_p && !other._p) || (_p && other._p && *_p == *other._p));
    }
    bool operator!=(const unique_ptr& other) const { return !(*this == other); }

  private:
    pointer _p;
};

template <typename T>
class array_unique_ptr {
  public:
    typedef T* pointer;
    typedef T& reference;

    array_unique_ptr(void) : _p(0) {}
    ~array_unique_ptr(void) {
        if (_p != 0) {
            delete[] _p;
        }
    }
    array_unique_ptr(pointer p) : _p(p) {}
    array_unique_ptr(array_unique_ptr& other) : _p(other._p) { other._p = 0; }
    array_unique_ptr& operator=(array_unique_ptr& rhs) {
        if (_p != 0) {
            delete[] _p;
        }
        _p = rhs._p;
        rhs._p = 0;
        return *this;
    }

    pointer release(void) {
        pointer ret = _p;
        _p = 0;
        return ret;
    }

    pointer get(void) const { return _p; }

    void reset(pointer p = pointer()) {
        if (_p != 0) {
            delete[] _p;
        }
        _p = p;
    }

    void swap(array_unique_ptr& other) {
        pointer tmp = _p;

        _p = other._p;
        other._p = tmp;
    }

    reference operator*() { return *_p; }

    pointer operator->() { return _p; }

  private:
    pointer _p;
};

class Time {
  public:
    ~Time(void);
    Time(Time const& src);
    Time& operator=(Time const& rhs);

    static Time now(void);

    std::string to_string(void);
    std::string to_http_string(void);

  private:
    time_t inner;
    char buf[26];

    Time(void);
    Time(time_t time_val);
};

} // namespace Utils

#endif
