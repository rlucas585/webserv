#pragma once

#include <fstream>
#include <iostream>

namespace Debug {

/// `Debug::cout` will never print to stdout when NDEBUG is defined
#ifdef NDEBUG
struct Cout : public std::ostream {
} static cout;
#else
extern std::ostream& cout;
#endif
} // namespace Debug
