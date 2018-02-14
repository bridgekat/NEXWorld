#ifndef WORLDGEN_H_
#define WORLDGEN_H_

#include <functional>
#include "vec.h"
#include "blockdata.h"

class WorldGen {
public:
	static std::function<void(const Vec3i&, BlockData*)> generator;
};

#endif
