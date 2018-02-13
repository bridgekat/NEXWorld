#include "chunkrenderer.h"

// 0-2   5
// |/   /|
// 1   3-4
const Vec3f vertices[36] = {
	// Right
	Vec3f(1.0f, 1.0f, 1.0f), Vec3f(1.0f, 0.0f, 1.0f), Vec3f(1.0f, 1.0f, 0.0f),
	Vec3f(1.0f, 0.0f, 1.0f), Vec3f(1.0f, 0.0f, 0.0f), Vec3f(1.0f, 1.0f, 0.0f),
	// Left
	Vec3f(0.0f, 1.0f, 0.0f), Vec3f(0.0f, 0.0f, 0.0f), Vec3f(0.0f, 1.0f, 1.0f),
	Vec3f(0.0f, 0.0f, 0.0f), Vec3f(0.0f, 0.0f, 1.0f), Vec3f(0.0f, 1.0f, 1.0f),
	// Top
	Vec3f(0.0f, 1.0f, 0.0f), Vec3f(0.0f, 1.0f, 1.0f), Vec3f(1.0f, 1.0f, 0.0f),
	Vec3f(0.0f, 1.0f, 1.0f), Vec3f(1.0f, 1.0f, 1.0f), Vec3f(1.0f, 1.0f, 0.0f),
	// Bottom
	Vec3f(0.0f, 0.0f, 1.0f), Vec3f(0.0f, 0.0f, 0.0f), Vec3f(1.0f, 0.0f, 1.0f),
	Vec3f(0.0f, 0.0f, 0.0f), Vec3f(1.0f, 0.0f, 0.0f), Vec3f(1.0f, 0.0f, 1.0f),
	// Front
	Vec3f(0.0f, 1.0f, 1.0f), Vec3f(0.0f, 0.0f, 1.0f), Vec3f(1.0f, 1.0f, 1.0f),
	Vec3f(0.0f, 0.0f, 1.0f), Vec3f(1.0f, 0.0f, 1.0f), Vec3f(1.0f, 1.0f, 1.0f),
	// Back
	Vec3f(1.0f, 1.0f, 0.0f), Vec3f(1.0f, 0.0f, 0.0f), Vec3f(0.0f, 1.0f, 0.0f),
	Vec3f(1.0f, 0.0f, 0.0f), Vec3f(0.0f, 0.0f, 0.0f), Vec3f(0.0f, 1.0f, 0.0f)
};

const Vec3f texCoords[6] = {
	Vec3f(0.0f, 1.0f, 0.0f), Vec3f(0.0f, 0.0f, 0.0f), Vec3f(1.0f, 1.0f, 0.0f),
	Vec3f(0.0f, 0.0f, 0.0f), Vec3f(1.0f, 0.0f, 0.0f), Vec3f(1.0f, 1.0f, 0.0f)
};

bool adjacentTest(BlockData curr, BlockData neighbor) {
	return curr.id != 0 && neighbor.id == 0;
}

VertexBuffer ChunkRenderer::buildRender() {
	const Chunk* c = mWorld.getChunkPtr(mPos);
	if (c == nullptr) return VertexBuffer();
	VertexArray va(262144, VertexFormat(2, 3, 0, 3));
	Vec3i::range(0, Chunk::Size, [&](const Vec3i& pos) {
		BlockData block[7] = {
			c->getBlock(pos),
			pos.x == Chunk::Size - 1 ? 0 : c->getBlock(pos + Vec3i(1, 0, 0)),
			pos.x == 0 ? 0 : c->getBlock(pos + Vec3i(-1, 0, 0)),
			pos.y == Chunk::Size - 1 ? 0 : c->getBlock(pos + Vec3i(0, 1, 0)),
			pos.y == 0 ? 0 : c->getBlock(pos + Vec3i(0, -1, 0)),
			pos.z == Chunk::Size - 1 ? 0 : c->getBlock(pos + Vec3i(0, 0, 1)),
			pos.z == 0 ? 0 : c->getBlock(pos + Vec3i(0, 0, -1)),
		};
		if (adjacentTest(block[0], block[1])) {
			// Right
			va.setColor({ 0.5f, 0.5f, 0.5f });
			for (int i = 0; i < 6; i++) {
				va.setTexture({ texCoords[i % 6].x, texCoords[i % 6].y });
				va.addVertex({ pos.x + vertices[i].x, pos.y + vertices[i].y, pos.z + vertices[i].z });
			}
		}
		if (adjacentTest(block[0], block[2])) {
			// Left
			va.setColor({ 0.5f, 0.5f, 0.5f });
			for (int i = 6; i < 12; i++) {
				va.setTexture({ texCoords[i % 6].x, texCoords[i % 6].y });
				va.addVertex({ pos.x + vertices[i].x, pos.y + vertices[i].y, pos.z + vertices[i].z });
			}
		}
		if (adjacentTest(block[0], block[3])) {
			// Top
			va.setColor({ 1.0f, 1.0f, 1.0f });
			for (int i = 12; i < 18; i++) {
				va.setTexture({ texCoords[i % 6].x, texCoords[i % 6].y });
				va.addVertex({ pos.x + vertices[i].x, pos.y + vertices[i].y, pos.z + vertices[i].z });
			}
		}
		if (adjacentTest(block[0], block[4])) {
			// Bottom
			va.setColor({ 1.0f, 1.0f, 1.0f });
			for (int i = 18; i < 24; i++) {
				va.setTexture({ texCoords[i % 6].x, texCoords[i % 6].y });
				va.addVertex({ pos.x + vertices[i].x, pos.y + vertices[i].y, pos.z + vertices[i].z });
			}
		}
		if (adjacentTest(block[0], block[5])) {
			// Front
			va.setColor({ 0.7f, 0.7f, 0.7f });
			for (int i = 24; i < 30; i++) {
				va.setTexture({ texCoords[i % 6].x, texCoords[i % 6].y });
				va.addVertex({ pos.x + vertices[i].x, pos.y + vertices[i].y, pos.z + vertices[i].z });
			}
		}
		if (adjacentTest(block[0], block[6])) {
			// Back
			va.setColor({ 0.7f, 0.7f, 0.7f });
			for (int i = 30; i < 36; i++) {
				va.setTexture({ texCoords[i % 6].x, texCoords[i % 6].y });
				va.addVertex({ pos.x + vertices[i].x, pos.y + vertices[i].y, pos.z + vertices[i].z });
			}
		}
	});
	return VertexBuffer(va);
}
