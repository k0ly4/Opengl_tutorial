#include "Resource_manager.h"

bool FileManager::read(std::string& data, const std::string& path) {
		// Этап №1: Получение исходного кода вершинного/фрагментного шейдера из переменной filePath
		std::ifstream file;
		// Убеждаемся, что объекты ifstream могут выбросить исключение
		file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		try
		{
			std::stringstream vShaderStream;

			file.open(path);
			vShaderStream << file.rdbuf();
			data = vShaderStream.str();
			file.close();
		}
		catch (std::ifstream::failure& e)
		{
			std::cout << "(!)ERROR::FILE_NOT_SUCCESFULLY_READ::" << path << std::endl;
			return 0;
		}
		return 1;
	}

//
//bool FileManager::loadTexture(const std::string& path, TextureShell* texture, glm::ivec2* size, bool gamma, int nrChannels_need) {
//
//		int nrChannels;
//		unsigned char* data = stbi_load(path.c_str(), &size->x, &size->y, &nrChannels, nrChannels_need);
//
//		if (!data) {
//			std::cout << "(!)ERROR::Failed to load texture (path== \"" << path << "\"" << std::endl;
//			return 0;
//		}
//		GLenum format, internal_format;
//		format = internal_format = GL_RED;
//
//		if (nrChannels == 3) {
//			internal_format = gamma ? GL_SRGB : GL_RGB;
//			format = GL_RGB;
//		}
//		else if (nrChannels == 4) {
//			internal_format = gamma ? GL_SRGB_ALPHA : GL_RGBA;
//			format = GL_RGBA;
//		}
//		texture->gen();
//		glTexture::bind2D(*texture);
//		glTexImage2D(GL_TEXTURE_2D, 0, internal_format, size->x, size->y, 0, format, GL_UNSIGNED_BYTE, data);
//		loaded_texture[path] = *texture;
//		stbi_image_free(data);
//		return 1;
//	}
//
//GeneralTexture* FileManager::getLoadedTexture(const std::string& path) {
//		auto texture = loaded_texture.find(path);
//		return texture == loaded_texture.end() ? 0 : &texture->second;
//	}
//
//bool FileManager::loadImage(GlImage* image, const std::string& path, int nrChannels_need) {
//		image->pixels = stbi_load(path.c_str(), &image->width, &image->height, &image->nrChannels, nrChannels_need);
//		if (!image->pixels) {
//			std::cout << "(!)ERROR::Failed to load image (path== \"" << path << "\"" << std::endl;
//			return 0;
//		}
//		return 1;
//	}
//
//bool FileManager::loadImage(GLFWimage* image, const std::string& path, int nrChannels_need) {
//		int nrChannels;
//		image->pixels = stbi_load(path.c_str(), &image->width, &image->height, &nrChannels, nrChannels_need);
//		if (!image->pixels) {
//			std::cout << "(!)ERROR::Failed to load image (path== \"" << path << "\"" << std::endl;
//			return 0;
//		}
//		return 1;
//	}
//
//std::map<std::string, GeneralTexture> FileManager::loaded_texture;
