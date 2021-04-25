#include "File.hpp"
#include "../../Str/src/Str.hpp"

#include <cerrno>
#include <cstring>
#include <fcntl.h>

File::File(void) : inner(FileDesc::init(0)) {}

File::File(int fd) : inner(FileDesc::init(fd)) {}

File::~File(void) {}

File::File(File const& other) { *this = other; }

File& File::operator=(File const& rhs) {
    if (this == &rhs) {
        return *this;
    }
    inner = rhs.inner; // Move semantics from FileDesc
    return *this;
}

File::Result File::open(const char* path, int flags) {
    int fd = ::open(path, flags);

    if (fd == -1) {
        return File::Result::Err(strerror(errno));
    }
    return File::Result::Ok(File(fd));
}

File::Result File::open(const char* path, int flags, mode_t mode) {
    int fd = ::open(path, flags, mode);

    if (fd == -1) {
        return File::Result::Err(strerror(errno));
    }
    return File::Result::Ok(File(fd));
}

Utils::RwResult File::write(const char* str) const { return inner.write(str); }

Utils::RwResult File::write(const void* buf, size_t count) const { return inner.write(buf, count); }

Utils::RwResult File::read(void* buf, size_t len) const { return inner.read(buf, len); }

Utils::RwResult File::read(std::string& str, size_t len) const { return inner.read(str, len); }
