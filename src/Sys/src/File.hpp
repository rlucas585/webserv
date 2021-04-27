#ifndef FILE_HPP
#define FILE_HPP

#include "../../Option/src/optional.hpp"
#include "../../Result/src/result.hpp"
#include "../../Str/src/Str.hpp"
#include "FileDesc.hpp"

#include <string>

class File {
  public:
    typedef Utils::result<File, std::string> Result;

  public:
    File(void);
    File(int fd);
    ~File(void);
    File(File const& other);
    File& operator=(File const& rhs);

    static Result open(const char* path, int flags);
    static Result open(const char* path, int flags, mode_t mode);

    Utils::RwResult write(const char* str) const;
    Utils::RwResult write(const void* buf, size_t count) const;
    Utils::RwResult write(std::string const& str) const;
    Utils::RwResult peek(void) const;

    Utils::RwResult read(void* buf, size_t len) const;
    Utils::RwResult read(std::string& str, size_t len) const;

    int fd(void) const;

  private:
    FileDesc inner;
};

#endif
