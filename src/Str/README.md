# A 'String Slice' C++ class 'Str'

`Str` is a C++ class roughly based on the 'string slice' from Rust, with
similarities to the std::basic\_string\_view of C++17.

`Str` contains a pointer to some bytes, and a length. These can be observed with
the `as_ptr()` and `length` methods. `Str` objects can be constructed from
char buffers, from std::string objects, and from other `Str` instances.

It is important to remember that the data that a `Str` points to must not be
mutated, freed, or dropped from scope for the lifetime of the `Str`, this will
cause invalid behaviour and is not checked by the C++ compiler.

The strength in the `Str` class lies in the ease with which buffers can be split
apart for parsing, without the need to allocate any additional heap memory.
If desired, a vector (or any other container) of `Str`'s can be generated for
simple parsing.

Additionally, an `Str` instance can be implicitly converted into
a std::string, allowing for simple generation of a std::vector<std::string>> if
desired. A single `Str` can also explicitly create an owned std::string by the
use of the `toString` member function.

### Example Usage

Create a `Str` and check if it is initialized.
```
Str   str;

if (!str.isInitialized())
  std::cout << "str has not yet been initialized, will throw" << "\n";

std::cout << "length: " << str.length() << "\n"; // Will throw exception
```

Create an empty `Str`, a `Str` pointing at a stack buffer, and a `Str` from 
a std::string.
```
Str   empty_str(""); // A `Str` pointing to a static char buffer on the stack

std::cout << empty_str.length() << "\n"; // 0

Str   stack_str("hello there");

std::cout << stack_str.length() << "\n"; // 11

std::string   owned_string("hello there");
Str           string_str(owned_string);

std::cout << owned_string.length() << "\n"; // 11
```

Output the contents of a `Str` through an output stream.
```
Str   str("I am a Str object");

std::cout << str << "\n"; // I am a Str object
```

Create a `Str` of specific length, or at an offset.
```
Str   str = Str::newSliceWithLength("hello there", 5);

std::cout << str.length() << "\n";  // 5
std::cout << str << "\n";           // hello

Str   str2 = Str::newSliceWithOffset("hello there", 6);

std::cout << str2.length() << "\n";  // 5
std::cout << str2 << "\n";           // there
```

`Str`'s can be compared to C strings and to std::strings.

```
Str         str("hello there");
std::string owned_string("hello there");
const char  *stack_str = "hello there";

std::cout << std::boolalpha;
std::cout << (str == owned_string) << "\n";   // true
std::cout << (str == stack_str) << "\n";      // true
```

`Str`'s can also be compared to other `Str`'s.
```
const char *	staticStr = "hello goodbye hello";
Str				str = Str::newSliceWithLength(staticStr, 5);
Str				str2 = Str::newSliceWithLengthAndOffset(staticStr, 5, 14);
Str				str3 = Str::newSliceWithLengthAndOffset(staticStr, 7, 6);

std::cout << std::boolalpha;
std::cout << (str == str2) << "\n";           // true
std::cout << (str == str3) << "\n";           // false
```

`Str`'s can be split using the `split` member function, into an instance of the
nested class `Str::Split`. `Str::Split` is an iterator that can be traversed,
or collected into a vector.
The `split` function can be called with a string delimiter, and the `Str` is
split at any occurrence of any of the characters within.
More sophisticated splitting (using a Pattern, or a needle for example) is
desirable, but not yet implemented.

Iterating over a Str:
```
Str         str = "192.168.0.1";
Str::Split  iter = str.split(".");

std::cout << iter.next() << "\n";   // 192
std::cout << iter.next() << "\n";   // 168
std::cout << iter.next() << "\n";   // 0
std::cout << iter.next() << "\n";   // 1
```

A `Str::Split` instance can also be created directly from a char buffer, to
avoid construction of a `Str` encompassing the entire buffer, which could
require an expensive call to strlen for large buffers.
```
const char  *stack_str = "I am a stack string, on the stack.";
Str::Split  iter(stack_str);

std::cout << iter.next() << "\n";   // I
std::cout << iter.next() << "\n";   // am
std::cout << iter.next() << "\n";   // a
std::cout << iter.next() << "\n";   // stack
std::cout << iter.next() << "\n";   // string,
std::cout << iter.next() << "\n";   // on
std::cout << iter.next() << "\n";   // the
std::cout << iter.next() << "\n";   // stack.
```

Collect a `Str::Split` into a vector of `Str`'s. (If owned std::strings are
required, then simply use <std::vector<std::string>> as the template parameter
for `collect`).
```
Str							    str = "The quick brown fox jumps over the lazy dog";
std::vector<Str>    vec = str.split().collect<std::vector<Str> >();
std::vector<Str>::iterator it = vec.begin();

std::cout << *it++ << std::endl; // The
std::cout << *it++ << std::endl; // quick
std::cout << *it++ << std::endl; // brown
std::cout << *it++ << std::endl; // fox
std::cout << *it++ << std::endl; // jumps
std::cout << *it++ << std::endl; // over
std::cout << *it++ << std::endl; // the
std::cout << *it++ << std::endl; // lazy
std::cout << *it++ << std::endl; // dog
```

##### Authors

* Ryan Lucas
