#include "ImageLoader.h"

std::shared_ptr <ResourceTexture2D> ImageLoader::loadTexture2D(const std::string& path, bool gamma, int nrChannels_need) {
	auto res = getSTBI(path, nrChannels_need);
	if (res == 0) {
		LOG(LogError, "Texture from %s - doesn't exist\n", path.c_str());
		return 0;
	}
	return std::make_shared<ResourceTexture2D>(*res, gamma);
}

std::shared_ptr<STBI_Resource> ImageLoader::loadSTBI(const std::string& path, int nrChannels_need) {
	std::shared_ptr<STBI_Resource> res = std::make_shared<STBI_Resource>(path.c_str(), nrChannels_need);
	if (res->data_.isEmpty()) {
		LOG(LogError,"Image from %s - doesn't exist\n", path.c_str());
		return 0;
	}
	return res;
}

int ImageLoader::savePNG(const std::string& path, const ResourceTexture2D& image) {
	byte* data = new byte[image.size_.x * image.size_.y * 4];
	sTexture::bind2D(image.id_);
	sTexture::getPixels(data);
	int status = savePNG(path.c_str(), image.size_,  4, data);
	if (status == 0) {
		LOG(LogError, "Failed to save png from ResourceTexture2D to %s\n", path.c_str());
	}
	delete[] data;
	return status;
}

std::map<std::string, std::shared_ptr <ResourceTexture2D>>	ImageLoader::resTextures;
std::map<std::string, std::shared_ptr <STBI_Resource>>		ImageLoader::resSTBI;