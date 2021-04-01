/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   FileDesc.hpp                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: rlucas <marvin@codam.nl>                     +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/03/19 20:00:41 by rlucas        #+#    #+#                 */
/*   Updated: 2021/04/01 23:32:47 by rlucas        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef FILEDESC_HPP
#define FILEDESC_HPP

#include "../../Result/src/result.hpp"
#include "../../Utils/src/rvalue.hpp"
#include <string>
#include <sys/types.h>

#ifndef _POSIX_SSIZE_MAX
#define _POSIX_SSIZE_MAX 32767
#endif

class FileDesc {
    typedef Utils::result<ssize_t, std::string> Result;

  public:
    FileDesc(void);
    ~FileDesc(void);
    FileDesc(FileDesc const& other);
    FileDesc& operator=(FileDesc const& rhs);
    FileDesc(Utils::rvalue<FileDesc> other);
    FileDesc& operator=(Utils::rvalue<FileDesc> rhs);

    static FileDesc init(int new_fd);

    int raw(void) const;
    FileDesc move(void);

    void writeToFile(const char* str) const;
    Result writeToFile(const void* buf, size_t count) const;

    void readFromFile(void* buf, size_t len) const;
    void readFromFile(std::string& str, size_t len) const;

  private:
    int fd;

    FileDesc(int new_fd);
    static const ssize_t READ_LIMIT = _POSIX_SSIZE_MAX;
};

#endif
