/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   FileDesc.hpp                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: rlucas <marvin@codam.nl>                     +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/03/19 20:00:41 by rlucas        #+#    #+#                 */
/*   Updated: 2021/03/19 22:58:45 by rlucas        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <limits>
#include <sys/types.h>
#include <string>

class FileDesc {
    public:
        ~FileDesc(void);
        FileDesc(FileDesc const& other);
        FileDesc &operator=(FileDesc const& rhs);

        static FileDesc        init(int new_fd);
        
        int     raw(void) const;

        void    writeToFile(const char* str) const;
        void    writeToFile(const void* buf, size_t count) const;

        void    readFromFile(void *buf, size_t len) const;
        void    readFromFile(std::string& str, size_t len) const;

    private:
        int     fd;

        FileDesc(void);
        FileDesc(int new_fd);

        static const unsigned int READ_LIMIT = std::numeric_limits<unsigned int>::max();
};
