/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   FileDesc.cpp                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: rlucas <marvin@codam.nl>                     +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/03/19 20:00:44 by rlucas        #+#    #+#                 */
/*   Updated: 2021/04/26 15:56:57 by rlucas        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "FileDesc.hpp"

#include "../../Utils/src/Utils.hpp"

#include <cerrno>
#include <cstring>
#include <string>
#include <unistd.h>

FileDesc::FileDesc(void) : fd(0) {}

FileDesc::FileDesc(int new_fd) : fd(new_fd) {}

FileDesc::~FileDesc(void) {
    if (fd > 2) {
        close(fd);
    }
}

// const_cast used to simulate move-semantics
FileDesc::FileDesc(FileDesc const& other) : fd(other.fd) {
    FileDesc& ref = const_cast<FileDesc&>(other);
    ref.fd = 0;
}

FileDesc& FileDesc::operator=(FileDesc const& rhs) {
    if (this == &rhs) {
        return *this;
    }
    FileDesc& ref = const_cast<FileDesc&>(rhs);
    if (fd > 2) {
        close(fd);
    }
    fd = rhs.fd;
    ref.fd = 0;
    return *this;
}

FileDesc FileDesc::init(int new_fd) { return FileDesc(new_fd); }

int FileDesc::raw(void) const { return fd; }

Utils::RwResult FileDesc::write(const char* str) const {
    return write(reinterpret_cast<const void*>(str),
                 Utils::min(Utils::strlen(str), static_cast<size_t>(READ_LIMIT)));
}

Utils::RwResult FileDesc::write(const void* buf, size_t count) const {
    ssize_t ret = ::write(fd, buf, count);
    if (ret == -1) {
        return Utils::RwResult::Err(strerror(errno));
    }
    return Utils::RwResult::Ok(ret);
}

Utils::RwResult FileDesc::read(void* buf, size_t len) const {
    ssize_t ret = ::read(fd, buf, len);
    if (ret == -1) {
        return Utils::RwResult::Err(strerror(errno));
    }
    return Utils::RwResult::Ok(ret);
}

Utils::RwResult FileDesc::read(std::string& str, size_t len) const {
    size_t previous_length = str.size();
    void* buf;
    ssize_t ret;

    str.resize(previous_length + len);
    buf = &str[previous_length];
    ret = ::read(fd, buf, len);
    if (ret == -1) {
        return Utils::RwResult::Err(strerror(errno));
    }
    if (static_cast<size_t>(ret) < len) {
        str.resize(previous_length + ret);
    }
    return Utils::RwResult::Ok(ret);
}
