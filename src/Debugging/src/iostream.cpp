#include "iostream.hpp"

namespace Debug {
#ifndef NDEBUG
std::ostream& cout = std::cout;
#endif
} // namespace Extra
