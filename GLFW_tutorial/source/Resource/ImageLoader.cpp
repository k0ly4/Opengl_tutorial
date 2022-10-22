#include "ImageLoader.h"

void ImageLoader::loadTexture(const std::string& path, bool gamma, int nrChannels_need) {

	STBI_Resource resource(path.c_str(),nrChannels_need);

	if (resource.isEmtpy())
		throw stbiResourceException(std::string("(!)ERROR::Texture from "+ path+ " - dont't found\n").c_str());

	rTextures_[path] = TextureResource(resource,gamma);
	
}

void ImageLoader::loadSTBI(const std::string& path, int nrChannels_need) {

	STBI_Resource resource(path.c_str(), nrChannels_need);

	if (resource.isEmtpy())
		throw stbiResourceException(std::string("(!)ERROR::Image from " + path + " - dont't found\n").c_str());

	rSTBI[path] = resource;

}

std::map<std::string, TextureResource> ImageLoader::rTextures_;
std::map<std::string, STBI_Resource> ImageLoader::rSTBI;