/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   FileDesc.hpp                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: rlucas <marvin@codam.nl>                     +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/03/19 20:00:41 by rlucas        #+#    #+#                 */
/*   Updated: 2021/04/03 14:45:37 by rlucas        ########   odam.nl         */
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
  public:
    FileDesc(void);
    ~FileDesc(void);
    FileDesc(FileDesc const& other);
    FileDesc& operator=(FileDesc const& rhs);

    static FileDesc init(int new_fd);

    int raw(void) const;

    Utils::RwResult writeToFile(const char* str) const;
    Utils::RwResult writeToFile(const void* buf, size_t count) const;

    Utils::RwResult readFromFile(void* buf, size_t len) const;
    Utils::RwResult readFromFile(std::string& str, size_t len) const;

  private:
    int fd;

    FileDesc(int new_fd);
    static const ssize_t READ_LIMIT = _POSIX_SSIZE_MAX;
};

#endif
