#include "chunkrenderer.h"
#include "blockrenderer.h"

bool adjacentTest(BlockData curr, BlockData neighbor, BlockModel::Facing invFacing) {
	return (curr.id != neighbor.id && BlockType::getBlockType(neighbor.id).translucent()) ||
		   !BlockRenderer::getModel(neighbor.id).full[invFacing];
}

std::pair<bool, VertexBuffer> ChunkRenderer::buildRender() {
	const Chunk* c[3][3][3];
	for (int x = -1; x <= 1; x++)
		for (int y = -1; y <= 1; y++)
			for (int z = -1; z <= 1; z++) {
				if (std::abs(x) + std::abs(y) + std::abs(z) > 1) continue;
				c[x + 1][y + 1][z + 1] = mWorld.getChunkPtr(mPos + Vec3i(x, y, z));
				if (c[x + 1][y + 1][z + 1] == nullptr)
					return std::make_pair(false, VertexBuffer());
			}

	VertexArray va(524288, VertexFormat(2, 3, 0, 3));
	Vec3i::range(0, Chunk::Size, [&](const Vec3i& pos) {
		BlockData curr = c[1][1][1]->getBlock(pos);
		if (BlockType::getBlockType(curr.id).visible()) {
			// Visible block, start rendering
			BlockData block[6] = {
				pos.x == Chunk::Size - 1 ? c[2][1][1]->getBlock(pos + Vec3i(1 - Chunk::Size, 0, 0)) : c[1][1][1]->getBlock(pos + Vec3i(1, 0, 0)),
				pos.x == 0 ? c[0][1][1]->getBlock(pos + Vec3i(Chunk::Size - 1, 0, 0)) : c[1][1][1]->getBlock(pos + Vec3i(-1, 0, 0)),
				pos.y == Chunk::Size - 1 ? c[1][2][1]->getBlock(pos + Vec3i(0, 1 - Chunk::Size, 0)) : c[1][1][1]->getBlock(pos + Vec3i(0, 1, 0)),
				pos.y == 0 ? c[1][0][1]->getBlock(pos + Vec3i(0, Chunk::Size - 1, 0)) : c[1][1][1]->getBlock(pos + Vec3i(0, -1, 0)),
				pos.z == Chunk::Size - 1 ? c[1][1][2]->getBlock(pos + Vec3i(0, 0, 1 - Chunk::Size)) : c[1][1][1]->getBlock(pos + Vec3i(0, 0, 1)),
				pos.z == 0 ? c[1][1][0]->getBlock(pos + Vec3i(0, 0, Chunk::Size - 1)) : c[1][1][1]->getBlock(pos + Vec3i(0, 0, -1)),
			};
			BlockRenderer::getModel(curr.id).render(va, pos, BlockModel::Center); // Center
			if (adjacentTest(curr, block[0], BlockModel::Left)) BlockRenderer::getModel(curr.id).render(va, pos, BlockModel::Right, 0.5f); // Right
			if (adjacentTest(curr, block[1], BlockModel::Right)) BlockRenderer::getModel(curr.id).render(va, pos, BlockModel::Left, 0.5f); // Left
			if (adjacentTest(curr, block[2], BlockModel::Bottom)) BlockRenderer::getModel(curr.id).render(va, pos, BlockModel::Top, 1.0f); // Top
			if (adjacentTest(curr, block[3], BlockModel::Top)) BlockRenderer::getModel(curr.id).render(va, pos, BlockModel::Bottom, 1.0f); // Bottom
			if (adjacentTest(curr, block[4], BlockModel::Back)) BlockRenderer::getModel(curr.id).render(va, pos, BlockModel::Front, 0.7f); // Front
			if (adjacentTest(curr, block[5], BlockModel::Front)) BlockRenderer::getModel(curr.id).render(va, pos, BlockModel::Back, 0.7f); // Back
		}
	});
	return std::make_pair(true, VertexBuffer(va, true));
}
