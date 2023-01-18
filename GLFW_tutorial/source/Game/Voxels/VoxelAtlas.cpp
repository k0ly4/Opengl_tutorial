#include "VoxelAtlas.h"

//VoxelAtlas-------------------------------------
/// <summary>
/// 
/// </summary>
bool ResourcePack::load(const std::string& directory) {
	//Json file
	JSON json;
	if (FileManager::read(directory+"atlas.json", json) == 0)return 0;
	//Texture
	ImageLoader::flipVerticallyOnLoad(1);
	if (texture_.load(directory +json.value("path", ""), 4) == 0) return 0;
	texture_.filter(tFilter::Nearest, tFilter::NearestMipmapNearest);
	//Parse
	sizeVoxel_ = json.value("size_uv",0);
	if (sizeVoxel_ == 0)
	{
		LOG(LogWarning, "VoxelAtlas::size_uv not set\n");
		sizeVoxel_ = 32;
	}

	uvSize_ = (float)sizeVoxel_ / (float)texture_.getSize().x;
	//scan
	blocks.resize(json.value("max_size_blocks",0));
	JSON& json_blocks = json["blocks"];
	for (size_t i = 0; i < blocks.size(); i++) {
		//name
		auto& block = json_blocks[std::to_string(i)];
		blocks[i].name = block["name"];
		//physGroup
		blocks[i].physGroup = block["physGroup"];
		//drawGroup
		blocks[i].drawGroup = block["drawGroup"];
		if (blocks[i].drawGroup == draw_non) {
			blocks[i].isEmission = 0;
		}
		else {
			//side
			auto& sides = block["side"];
			for (size_t side = 0; side < 6; side++) blocks[i].idSide[side] = sides[side];
			//emission
			auto& emission = block["emission"];
			blocks[i].emission = Uint8RGB(emission[0], emission[1], emission[2]); blocks[i].setEmissionFlag();
		}
	}
	//uv
	voxelInSide = texture_.getSize().x / sizeVoxel_;
	uv.resize(voxelInSide * voxelInSide);
	for (size_t i = 0; i < uv.size(); i++) uv[i] = getUV(i);

	icon.setTexture(VoxPack::get()->getTexture());
	icon.transform.setPosition(glm::vec2(0.f));
	icon.transform.setScale(2.f, 2.f);
	return 1;
}

const ResourcePack* VoxPack::res = 0;