
#ifndef RESOURCE_MANAGER_H
#define RESOURCE_MANAGER_H
#include <string>
#include <list>
#include"stb_image.h"
#include <fstream>
#include <sstream>
#include "ContextTexture.h"
class FileManager {

public:
	static bool read(std::string& data, const std::string& path);
	static bool loadTexture(const std::string& path, GeneralTexture* texture, glm::ivec2* size, bool gamma = 0, int nrChannels_need = 0);
	static GeneralTexture* getLoadedTexture(const std::string& path);
	static bool loadImage(GlImage* image, const std::string& path, int nrChannels_need = 0);
	static bool loadImage(GLFWimage* image, const std::string& path, int nrChannels_need = 0);
	static inline void freeDataImage(unsigned char* data) {
		stbi_image_free(data);
	}
	static inline void free() {
		loaded_texture.clear();
	}
private:
	static std::map<std::string, GeneralTexture> loaded_texture;
	FileManager() {}
	~FileManager() {}
};


#endif