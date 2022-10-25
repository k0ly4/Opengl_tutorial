#include "VoxelAtlas.h"

//VoxelAtlas-------------------------------------
/// <summary>
/// 
/// </summary>
void VoxelAtlas::load(const std::string& path, size_t sizeVoxelUV) {
	texture_.getPath(path, 4);
	texture_.filter(TextureFilter::Nearest, TextureFilter::NearestMipmapNearest);
	sizeVoxel_ = sizeVoxelUV;
	uvSize_ = (float)sizeVoxel_ / (float)texture_.getSize().x;

	//scan
	scan.resize(id_max);
	scan[id_turf].id[bottom] = uv_earth;
	scan[id_turf].id[front] = scan[id_turf].id[left] = scan[id_turf].id[right] = scan[id_turf].id[back] = uv_side_turf;
	scan[id_turf].id[top] = uv_turf;

	scan[id_earth].setSolid(uv_earth);
	//uv
	uv.resize(uv_max);
	for (size_t i = 0; i < uv_max; i++) {
		uv[i] = getUV(i);
	}
}
