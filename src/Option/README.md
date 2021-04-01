# A C++ 'optional' class

`Utils::optional` is a C++98 compatible reimplementation of `std::optional` from
C++17.

Being limited to C++98, there are some extreme deficiencies in
`Utils::optional` - mostly centered around the lack of move semantics in C++98,
thus requiring many copy operations.

`Utils::optional` is a container that contains either uninitialized data,
or a value (supplied by a template parameter). `optional` should be used instead
of the return of a pointer from functions, instead of NULL or nullptr, as it
forces the programmer to consider what to do when a value is not returned from
a function.

The inner value can be accessed using `value()`, and """removed""" (not moved
because no move semantics) using `unwrap()`. `unwrap()` will throw if there is
no value (as will `value()`). `expect()` can also be used to remove the value,
and a custom error message can be supplied should an error occur.

### Example Usage

Create an `optional` and check if the value is initialized.
```
Utils::optional<int>  opt;

std::cout << std::boolalpha << opt.has_value() << std::endl; // false
```

Create an `optional` with `make_optional()`
```
Utils::optional<std::string> opt = Utils::make_optional(std::string("hello there"));
```

Get a reference of the value inside of an optional with `value()`, a copy with
`unwrap()` and `expect()`.
```
Utils::optional<int>  opt5(5);
Utils::optional<int>  opt21(21);
Utils::optional<int>  opt42(42);

int x = opt5.value();
std::cout << x << std::endl;  // 5
std::cout << std::boolalpha << opt5.has_value() << std::endl; // true

int y = opt21.unwrap();
std::cout << y << std::endl;  // 21
std::cout << std::boolalpha << opt21.has_value() << std::endl; // false

int z = opt42.expect("No value in opt42");
std::cout << z << std::endl;  // 42
std::cout << std::boolalpha << opt42.has_value() << std::endl; // false
```

Get a copy of the value in an `optional`, or a default value, with `value_or()`.
```
Utils::optional<int> opt(20);

std::cout << opt.value_or(15) << std::endl; // 20
opt.reset();
std::cout << opt.value_or(15) << std::endl; // 15
```
