#include <pluginapi.h>
#include <sstream>
#include "blocktexture.h"
#include "blockrenderer.h"

namespace PluginAPI {
	extern std::string currPluginName;

	BlockModel::Vertex convertVertex(const NWvertex& src) {
		std::pair<float, float> uv = BlockTexture::getTextureCoord(src.tex).map(src.u, src.v);
		return BlockModel::Vertex(uv.first, uv.second, Vec3f(src.nx, src.ny, src.nz), Vec3f(src.x, src.y, src.z));
	}
}

extern "C" {

	NWtextureid NWAPICALL nwRegisterTexture(const char* filename) {
		NWtextureid res = BlockTexture::registerTexture(std::string(filename));
		std::stringstream ss;
		ss << "[Plugin]" + PluginAPI::currPluginName << " registered block texture " << res << " from \"" << filename << "\"";
		LogInfo(ss.str());
		return res;
	}

	void NWAPICALL nwRegisterBlockModel(NWblockid id, int count, const NWvertex* vertices,
										bool rightFull, bool leftFull, bool topFull, bool bottomFull, bool frontFull, bool backFull) {
		bool faceFull[7] = { false, rightFull, leftFull, topFull, bottomFull, frontFull, backFull };
		std::vector<BlockModel::Vertex> mesh;
		for (int i = 0; i < count; i++) mesh.push_back(PluginAPI::convertVertex(vertices[i]));
		BlockRenderer::registerModel(id, mesh, faceFull);
		std::stringstream ss;
		ss << "[Plugin]" + PluginAPI::currPluginName << " registered block model for block ID " << id;
		LogInfo(ss.str());
	}

}
