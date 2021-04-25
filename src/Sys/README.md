# 'System' helper classes in C++

This module contains abstracted classes for handling files, reading
and writing, with more abstracted classes planned in future.

### FileDesc

`FileDesc` is a simple wrapper around a file descriptor. The integer value can
always be readily accessed by calling `raw()` on an instance of the class.
The primary benefit of `FileDesc` is that `close()` is called on its file descriptor
in its destructor - so files will be closed by default when a `FileDesc` goes
out of scope.

While `FileDesc` can be used by itself, it's primary use is as a member variable
in more abstracted classes, such as `File` or `Socket`.

### File

`File` is an abstracted class for interacting with files. It provides an `open()`
constructor, which can take a filename, flags, and mode as parameters. It also
provides the `read()` and `write()` member functions, for easy reading and writing
from files - although when reading, it is more appropriate for performance to
wrap a `File` inside of a `BufReader` for more effective read operations.

#### Examples

Create (or overwrite) a file named "output.txt", and write to it.
```
#include "File.hpp"

File::Result file_res = File::open("output.txt", O_CREAT | O_WRONLY | O_TRUNC, 0644);

EXPECT_TRUE(file_res.is_ok());

File file = file_res.unwrap();
EXPECT_EQ(file.write("hello there\n, welcome!\n"), Utils::RwResult::Ok(23));
```

### BufReader

`BufReader` is a wrapper around any class that implements the `read()` and `write()`
functions - such as a `File` or a `Socket`. BufReader enhances the performance
of read operations, by reading a large number of bytes to an internal buffer,
and then returning them, rather than performing many small read operations.

`BufReader` also provides additional functionality around a Reader, such as
the `read_until()` member function, which will read all characters up to a delimiter
into a provided string, and the `read_line()` function, which will read the next
line of the Reader into a string.

#### Examples

Read from a file line by line
```
File::Result file_res = File::open("output.txt", O_RDONLY, 0644);

ASSERT_TRUE(file_res.is_ok());
File file = file_res.unwrap();

BufReader<File> reader = BufReader<File>::init(file);
std::string line;

Utils::RwResult res = reader.read_line(line);

EXPECT_FALSE(res.is_err());
EXPECT_NE(res.unwrap(), 0);
EXPECT_EQ(line, "hello there\n");

line.clear();
res = reader.read_line(line);

EXPECT_FALSE(res.is_err());
EXPECT_NE(res.unwrap(), 0);
EXPECT_EQ(line, ", welcome!\n");

line.clear();
res = reader.read_line(line);

EXPECT_FALSE(res.is_err());
EXPECT_EQ(res.unwrap(), 0);
EXPECT_EQ(line, "");
```
