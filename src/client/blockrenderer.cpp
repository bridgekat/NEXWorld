#include "blockrenderer.h"

std::vector<BlockModel> BlockRenderer::mBlockModels;

BlockModel::BlockModel(const std::vector<Vertex>& mesh, bool faceFull[7]) {
	memcpy(full, faceFull, sizeof(full));
	const float Eps = 1e-4;
	if (mesh.size() % 3 != 0) {
		LogWarning("Unable to build block mesh: vertex count is not a multiple of 3");
		return;
	}
	for (size_t i = 0; i < mesh.size(); i += 3) {
		// Classify current triangle
		Facing facing = Center;
		if (mesh[i].pos.x > 1.0f - Eps && mesh[i + 1].pos.x > 1.0f - Eps && mesh[i + 2].pos.x > 1.0f - Eps) facing = Right;
		else if (mesh[i].pos.x < Eps && mesh[i + 1].pos.x < Eps && mesh[i + 2].pos.x < Eps) facing = Left;
		else if (mesh[i].pos.y > 1.0f - Eps && mesh[i + 1].pos.y > 1.0f - Eps && mesh[i + 2].pos.y > 1.0f - Eps) facing = Top;
		else if (mesh[i].pos.y < Eps && mesh[i + 1].pos.y < Eps && mesh[i + 2].pos.y < Eps) facing = Bottom;
		else if (mesh[i].pos.z > 1.0f - Eps && mesh[i + 1].pos.z > 1.0f - Eps && mesh[i + 2].pos.z > 1.0f - Eps) facing = Front;
		else if (mesh[i].pos.z < Eps && mesh[i + 1].pos.z < Eps && mesh[i + 2].pos.z < Eps) facing = Back;
		vertices[facing].push_back(mesh[i]);
		vertices[facing].push_back(mesh[i + 1]);
		vertices[facing].push_back(mesh[i + 2]);
	}
}
