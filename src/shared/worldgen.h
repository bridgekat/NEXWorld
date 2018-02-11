#ifndef WORLDGEN_H_
#define WORLDGEN_H_

// Not in use

#include <functional>
#include "blockdata.h"

class WorldGen {
public:
	std::function<void(const Vec3i&, BlockData*)> generator;
};

#endif
