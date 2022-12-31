#include "ImageLoader.h"

bool ImageLoader::loadTexture(const std::string& path, bool gamma, int nrChannels_need) {

	STBI_Resource resource(path.c_str(),nrChannels_need);
	if (resource.isEmtpy()) {
		LOG(LogError, "Texture from %s - dont't found\n", path.c_str());
		return 0;
	}
	rTextures_[path] = TextureResource(resource,gamma);
	return 1;
}

bool ImageLoader::loadSTBI(const std::string& path, int nrChannels_need) {

	STBI_Resource resource(path.c_str(), nrChannels_need);
	
	if (resource.isEmtpy()) {
		LOG(LogError,"Image from %s - dont't found\n", path.c_str());
		return 0;
	}

	rSTBI[path] = resource;
	return 1;
}

std::map<std::string, TextureResource> ImageLoader::rTextures_;
std::map<std::string, STBI_Resource> ImageLoader::rSTBI;