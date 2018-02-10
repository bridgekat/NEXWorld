#ifndef DEBUG_H_
#define DEBUG_H_

#include <cassert>

inline void Assert(bool expr) { assert(expr); }

#undef assert

#endif // !DEBUG_H_
