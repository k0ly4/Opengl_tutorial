
#ifndef RESOURCE_MANAGER_H
#define RESOURCE_MANAGER_H
#include <string>
#include <list>
#include"stb_image.h"
#include "Math.h"

class GeneralTexture {
protected:
	friend class glTexture;
	struct g_texture {
		unsigned int id;
		g_texture() {
			glGenTextures(1, &id);
		}
		~g_texture() {
			glDeleteTextures(1, &id);
		}
	};
	friend class Texture2D;
	friend class g_Texture2D;
	friend class g_ArrayTexture2D;
	std::shared_ptr<g_texture> texture;
public:
	void gen() {
		texture = std::make_unique<g_texture>();
	}
	inline unsigned int getID() const {
		return texture.get()->id;
	}
};
class glTexture {
	static unsigned int last;
	/*static unsigned int last2d;
	static unsigned int lastCube;
	static unsigned int last2dArray;*/
	static GLenum active_unit;
	glTexture() {}
	~glTexture() {}
public:
	static inline void active(size_t text_unit) {
		if (text_unit != active_unit) {
			active_unit = text_unit;
			glActiveTexture(active_unit);
		}
	}
	static inline void bind2D(unsigned int texture) {
		if (texture != last) {
			last = texture;
			glBindTexture(GL_TEXTURE_2D, last);
		}
	}
	static inline void bind2D(GeneralTexture& texture) {
		bind2D(texture.getID());
	}
	static inline void bind2D(GeneralTexture& texture, size_t text_unit) {
		active(text_unit);
		bind2D(texture.getID());
	}
	static inline void bind2DArray(unsigned int texture) {
		if (texture != last) {
			last = texture;
			glBindTexture(GL_TEXTURE_2D_ARRAY, last);
		}
	}
	static inline void bindCubeMap(unsigned int texture) {
		if (texture != last) {
			last = texture;
			glBindTexture(GL_TEXTURE_CUBE_MAP, last);
		}
	}
	static inline void bindCubeMap(GeneralTexture& texture) {
		bindCubeMap(texture.getID());
	}
};
unsigned int glTexture::last = 0;
//unsigned int glTexture::last2d = 0;
//unsigned int glTexture::last2dArray = 0;
//unsigned int glTexture::lastCube = 0;
GLenum glTexture::active_unit = 0;

class GlImage:public GLFWimage {
public:
	int nrChannels;
	GLenum format() {
		if (nrChannels == 3)
			return GL_RGB;
		else if (nrChannels == 4)
			return  GL_RGBA;
		else return GL_RED;
	}
	GLenum internal_format(bool gamma) {
		if (nrChannels == 3)
			return gamma ? GL_SRGB : GL_RGB;
		else if (nrChannels == 4)
			return  gamma ? GL_SRGB_ALPHA : GL_RGBA;
		else return GL_RED;
	}
	~GlImage() {
		stbi_image_free(pixels);
	}
};
class FileManager {
	static std::map<std::string, GeneralTexture> loaded_texture;
	FileManager() {}
	~FileManager() {}
public:
	static bool read(std::string& data,const std::string& path) {
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
	static bool loadTexture(const std::string& path, GeneralTexture* texture, glm::ivec2* size, bool gamma=0, int nrChannels_need = 0) {

		int nrChannels;
		unsigned char* data = stbi_load(path.c_str(), &size->x, &size->y, &nrChannels, nrChannels_need);
		
		if (!data) {
			std::cout << "(!)ERROR::Failed to load texture (path== \"" << path<<"\"" << std::endl;
			return 0;
		}
		GLenum format, internal_format;
		format = internal_format = GL_RED;

		if (nrChannels == 3) {
			internal_format = gamma ? GL_SRGB : GL_RGB;
			format = GL_RGB;
		}
		else if (nrChannels == 4) {
			internal_format = gamma ? GL_SRGB_ALPHA: GL_RGBA;
			format = GL_RGBA;
		}
		texture->gen();
		glTexture::bind2D(*texture);
		glTexImage2D(GL_TEXTURE_2D, 0, internal_format, size->x, size->y, 0, format, GL_UNSIGNED_BYTE, data);
		loaded_texture[path] = *texture;
		stbi_image_free(data);
		return 1;
	}
	static GeneralTexture* getLoadedTexture(const std::string& path) {
		auto texture = loaded_texture.find(path);
		return texture == loaded_texture.end() ? 0 : &texture->second;
	}
	static bool loadImage(GlImage* image,const std::string& path, int nrChannels_need = 0) {
		image->pixels = stbi_load(path.c_str(), &image->width, &image->height, &image->nrChannels, nrChannels_need);
		if (!image->pixels) {
			std::cout << "(!)ERROR::Failed to load image (path== \"" << path << "\"" << std::endl;
			return 0;
		}
		return 1;
	}
	static bool loadImage(GLFWimage* image, const std::string& path, int nrChannels_need = 0) {
		int nrChannels;
		image->pixels = stbi_load(path.c_str(), &image->width, &image->height, &nrChannels, nrChannels_need);
		if (!image->pixels) {
			std::cout << "(!)ERROR::Failed to load image (path== \"" << path << "\"" << std::endl;
			return 0;
		}
		return 1;
	}
	static inline void freeDataImage(unsigned char* data) {
		stbi_image_free(data);
	}
	static inline void free() {
		loaded_texture.clear();
	}
};
std::map<std::string, GeneralTexture> FileManager::loaded_texture;
#endif