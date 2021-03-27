/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   FileDesc.cpp                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: rlucas <marvin@codam.nl>                     +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/03/19 20:00:44 by rlucas        #+#    #+#                 */
/*   Updated: 2021/03/27 14:48:58 by rlucas        ########   odam.nl         */
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
    fd = rhs.fd;
    ref.fd = 0;
    return *this;
}

FileDesc::FileDesc(Utils::rvalue<FileDesc> other) : fd(other.get().fd) { other.get().fd = 0; }

FileDesc& FileDesc::operator=(Utils::rvalue<FileDesc> rhs) {
    if (this == &rhs.get()) {
        return *this;
    }
    fd = rhs.get().fd;
    rhs.get().fd = 0;
    return *this;
}

FileDesc FileDesc::init(int new_fd) { return FileDesc(new_fd); }

int FileDesc::raw(void) const { return fd; }

// "move" semantics for FileDesc
FileDesc FileDesc::move(void) {
    FileDesc ret(fd);
    fd = 0;
    return ret;
}

void FileDesc::writeToFile(const char* str) const {
    writeToFile(reinterpret_cast<const void*>(str), Utils::min(Utils::strlen(str), static_cast<size_t>(READ_LIMIT)));
}

void FileDesc::writeToFile(const void* buf, size_t count) const {
    if (write(fd, buf, count) == -1) {
        throw Utils::runtime_error(std::string("FileDesc::writeToFile Error: ") + strerror(errno));
    }
}

void FileDesc::readFromFile(void* buf, size_t len) const {
    if (read(fd, buf, len) == -1) {
        throw Utils::runtime_error(std::string("FileDesc::writeToFile Error: ") + strerror(errno));
    }
}

void FileDesc::readFromFile(std::string& str, size_t len) const {
    size_t previous_length = str.size();
    void* buf;
    ssize_t ret;

    str.resize(previous_length + len);
    buf = &str[previous_length];
    ret = read(fd, buf, len);
    if (ret == -1) {
        throw Utils::runtime_error(std::string("FileDesc::writeToFile Error: ") + strerror(errno));
    }
    if (static_cast<size_t>(ret) < len) {
        str.resize(previous_length + ret);
    }
}
