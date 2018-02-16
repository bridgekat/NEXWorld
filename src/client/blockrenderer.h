#ifndef BLOCKRENDERER_H_
#define BLOCKRENDERER_H_

#include <vector>
#include <sstream>
#include <vec.h>
#include <logger.h>
#include <blocktype.h>
#include "vertexarray.h"

class BlockModel {
public:
	enum Facing: unsigned int {
		Center = 0, Right, Left, Top, Bottom, Front, Back
	};

	struct Vertex {
		float u, v;
		Vec3f normal, pos;

		Vertex() = default;
		Vertex(float u_, float v_, const Vec3f& normal_, const Vec3f& pos_):
			u(u_), v(v_), normal(normal_), pos(pos_) {}
	};

	std::vector<Vertex> vertices[7];
	bool full[7];
	// We can't detect full faces automatically now, so you need to specify manually...
	// What? Software rasterizer???
	// TODO: software rasterizer
	BlockModel() = default;
	BlockModel(const std::vector<Vertex>& mesh, bool faceFull[7]);

	void render(VertexArray& va, const Vec3i& pos, Facing facing, float color = 1.0f) const {
		for (size_t i = 0; i < vertices[facing].size(); i++) {
			const Vertex& curr = vertices[facing][i];
			if (va.format().textureCount >= 2) va.setTexture({ curr.u, curr.v });
			if (va.format().colorCount >= 3) va.setColor({ color, color, color });
			if (va.format().normalCount >= 3) va.setNormal({ curr.normal.x, curr.normal.y, curr.normal.z });
			if (va.format().coordinateCount >= 3) va.addVertex({ pos.x + curr.pos.x, pos.y + curr.pos.y, pos.z + curr.pos.z });
		}
	}
};

class BlockRenderer {
public:
	static void postInit() {
		mBlockModels.clear();
		mBlockModels.resize(BlockType::count());
	}

	static void registerModel(unsigned int id, const std::vector<BlockModel::Vertex>& mesh, bool faceFull[7]) {
		if (id >= mBlockModels.size()) {
			std::stringstream ss;
			ss << "Unable to register block model with ID " << id << ": out of ID range";
			LogWarning(ss.str());
			return;
		}
		mBlockModels[id] = BlockModel(mesh, faceFull);
	}

	static const BlockModel& getModel(unsigned int id) {
		if (id >= mBlockModels.size()) {
			std::stringstream ss;
			ss << "Failed to find block model with ID " << id << ": out of ID range";
			LogWarning(ss.str());
			// TODO: add a null block
			return mBlockModels[0];
		}
		return mBlockModels[id];
	}

private:
	static std::vector<BlockModel> mBlockModels;
};

#endif
