#include "ImageLoader.h"

std::shared_ptr <ResourceTexture2D> ImageLoader::loadTexture(const std::string& path, bool gamma, int nrChannels_need) {
	STBI_Resource resource(path.c_str(),nrChannels_need);
	if (resource.data_.isEmpty()) {
		LOG(LogError, "Texture from %s - didn't found\n", path.c_str());
		return 0;
	}
	rTextures_[path] = std::make_shared<ResourceTexture2D>(resource,gamma);
	return rTextures_[path];
}

bool ImageLoader::loadSTBI(const std::string& path, int nrChannels_need) {

	STBI_Resource resource(path.c_str(), nrChannels_need);
	
	if (resource.data_.isEmpty()) {
		LOG(LogError,"Image from %s - dont't found\n", path.c_str());
		return 0;
	}

	rSTBI[path] = resource;
	return 1;
}

std::map<std::string, std::shared_ptr <ResourceTexture2D>>	ImageLoader::rTextures_;
std::map<std::string, STBI_Resource>						ImageLoader::rSTBI;