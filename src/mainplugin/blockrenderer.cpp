#include "blockrenderer.h"

#include <iostream>

NWtextureid BlockRenderer::GrassTop, BlockRenderer::GrassSide, BlockRenderer::Dirt, BlockRenderer::Rock, BlockRenderer::Sand, BlockRenderer::Water;

void BlockRenderer::bindFullBlockModel(NWblockid id, NWtextureid tex[6]) {
	// 0-2   5
	// |/   /|
	// 1   3-4
	const float coords[36 * 3] = {
		// Right
		1.0f, 1.0f, 1.0f,
		1.0f, 0.0f, 1.0f,
		1.0f, 1.0f, 0.0f,
		1.0f, 0.0f, 1.0f,
		1.0f, 0.0f, 0.0f,
		1.0f, 1.0f, 0.0f,
		// Left
		0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 1.0f,
		0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f,
		0.0f, 1.0f, 1.0f,
		// Top
		0.0f, 1.0f, 0.0f,
		0.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 0.0f,
		0.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 0.0f,
		// Bottom
		0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 1.0f,
		// Front
		0.0f, 1.0f, 1.0f,
		0.0f, 0.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		0.0f, 0.0f, 1.0f,
		1.0f, 0.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		// Back
		1.0f, 1.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f
	};

	const float texCoords[6 * 2] = {
		0.0f, 0.0f,
		0.0f, 1.0f,
		1.0f, 0.0f,
		0.0f, 1.0f,
		1.0f, 1.0f,
		1.0f, 0.0f
	};

	NWvertex vertices[36];
	for (int i = 0; i < 6; i++) {
		for (int j = 0; j < 6; j++) {
			vertices[i * 6 + j].tex = tex[i];
			vertices[i * 6 + j].u = texCoords[j * 2];
			vertices[i * 6 + j].v = texCoords[j * 2 + 1];
			vertices[i * 6 + j].x = coords[i * 18 + j * 3];
			vertices[i * 6 + j].y = coords[i * 18 + j * 3 + 1];
			vertices[i * 6 + j].z = coords[i * 18 + j * 3 + 2];
		}
	}
	nwRegisterBlockModel(id, 36, vertices, true, true, true, true, true, true);
}

void BlockRenderer::registerTextures() {
	const std::string TexturePath = "../Textures/NEXWorldGame/";
	GrassTop = nwRegisterTexture((TexturePath + "GrassTop.png").c_str());
	GrassSide = nwRegisterTexture((TexturePath + "GrassSide.png").c_str());
	Dirt = nwRegisterTexture((TexturePath + "Dirt.png").c_str());
	Rock = nwRegisterTexture((TexturePath + "Rock.png").c_str());
	Sand = nwRegisterTexture((TexturePath + "Sand.png").c_str());
	Water = nwRegisterTexture((TexturePath + "Water.png").c_str());
}

void BlockRenderer::registerBlockModels() {
	// Add entries here! x2
	NWtextureid grassTex[6] = { GrassSide, GrassSide, GrassTop, Dirt, GrassSide, GrassSide };
	NWtextureid rockTex[6] = { Rock, Rock, Rock, Rock, Rock, Rock };
	NWtextureid dirtTex[6] = { Dirt, Dirt, Dirt, Dirt, Dirt, Dirt };
	NWtextureid sandTex[6] = { Sand, Sand, Sand, Sand, Sand, Sand };
	NWtextureid waterTex[6] = { Water, Water, Water, Water, Water, Water };
	bindFullBlockModel(Block::Grass, grassTex);
	bindFullBlockModel(Block::Rock, rockTex);
	bindFullBlockModel(Block::Dirt, dirtTex);
	bindFullBlockModel(Block::Sand, sandTex);
	bindFullBlockModel(Block::Water, waterTex);
}
