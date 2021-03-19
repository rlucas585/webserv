/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   FileDesc.cpp                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: rlucas <marvin@codam.nl>                     +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/03/19 20:00:44 by rlucas        #+#    #+#                 */
/*   Updated: 2021/03/19 23:06:14 by rlucas        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "FileDesc.hpp"

#include <unistd.h>
#include <cstring>

FileDesc::FileDesc(int new_fd) : fd(new_fd) {}

FileDesc::~FileDesc(void) {
    close(fd);
}

FileDesc::FileDesc(FileDesc const& other) {
    *this = other;
}

FileDesc &FileDesc::operator=(FileDesc const& rhs) {
    if (this == &rhs) { return *this; }
    fd = rhs.fd;
    return *this;
}

FileDesc        FileDesc::init(int new_fd) {
    return FileDesc(new_fd);
}

int             FileDesc::raw(void) const {
    return fd;
}

// TODO add exceptions in case of errors.
void            FileDesc::writeToFile(const char* str) const {
    // TODO change strlen(str) to Utils::min(Utils::strlen(str), READ_LIMIT));
    writeToFile(reinterpret_cast<const void*>(str), strlen(str));
}

void            FileDesc::writeToFile(const void* buf, size_t count) const {
    write(fd, buf, count);
}

void            FileDesc::readFromFile(void *buf, size_t len) const {
    read(fd, buf, len);
}

void            FileDesc::readFromFile(std::string& str, size_t len) const {
    size_t      previous_length = str.size();
    void        *buf;
    ssize_t     ret;
    
    str.resize(previous_length + len);
    buf = &str[previous_length];
    ret = read(fd, buf, len);
    if (ret == -1) { ; } // TODO change this to throw
    if (static_cast<size_t>(ret) < len) { str.resize(previous_length + ret); }
}
