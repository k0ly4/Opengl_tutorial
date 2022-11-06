#include "VoxelAtlas.h"

//VoxelAtlas-------------------------------------
/// <summary>
/// 
/// </summary>
void ResourceVoxelPack::load(const std::string& path, size_t sizeVoxelUV) {
	texture_.getPath(path, 4);
	texture_.filter(TextureFilter::Nearest, TextureFilter::NearestMipmapNearest);
	sizeVoxel_ = sizeVoxelUV;
	uvSize_ = (float)sizeVoxel_ / (float)texture_.getSize().x;

	//scan
	blocks.resize(id_max);
	blocks[id_turf].idSide[bottom] = uv_earth;
	blocks[id_turf].idSide[front] = blocks[id_turf].idSide[left] = blocks[id_turf].idSide[right] = blocks[id_turf].idSide[back] = uv_side_turf;
	blocks[id_turf].idSide[top] = uv_turf;

	blocks[id_earth].setSolid(uv_earth);
	blocks[id_light].setSolid(uv_light);
	//uv
	uv.resize(uv_max);
	for (size_t i = 0; i < uv_max; i++) {
		uv[i] = getUV(i);
	}
}

//VoxelAtlas-------------------------------------
/// <summary>
/// 
/// </summary>
bool ResourceVoxelPack::load(const std::string& directory) {
	JSON json;

	if (FileManager::read(directory+"atlas.json", json) == 0)return 0;
	if (texture_.getPath(directory +json.value("path", ""), 4) == 0) return 0;

	texture_.filter(TextureFilter::Nearest, TextureFilter::NearestMipmapNearest);
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
		//side
		auto& sides = block["side"];
		for (size_t side = 0; side < 6; side++) {
			blocks[i].idSide[side] = sides[side];
		}
		//emission
		auto& emission = block["emission"];
		blocks[i].emission = Uint8RGB(emission[0], emission[1], emission[2]);
		blocks[i].setEmissionFlag();
		blocks[i].drawGroup = block["drawGroup"];
		
	}
	//uv
	size_t size_side = texture_.getSize().x / sizeVoxel_;
	uv.resize(size_side * size_side);
	for (size_t i = 0; i < uv.size(); i++) {
		uv[i] = getUV(i);
	}

	return 1;
}

const ResourceVoxelPack* VoxelPack::pack = 0;