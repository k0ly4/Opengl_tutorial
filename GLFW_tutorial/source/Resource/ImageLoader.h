#ifndef IMAGE_LOADER_H
#define IMAGE_LOADER_H

#include"stb_image.h"
#include <string>
#include <list>
#include <fstream>
#include <sstream>

#include "Graphic/ContextTexture.h"
#include "System/Exception.h"

/// <summary>
/// STBI_Data
/// </summary>
class STBI_Data {

	struct Data {

		unsigned char* data;

		Data(unsigned char* data_)
			:data(data_) {}

		~Data() {
			stbi_image_free(data);
		}
	};

public:

	STBI_Data(unsigned char* data):
	data_(std::make_shared<Data>(data)) 
	{}

	STBI_Data() 
	{}

	void set(unsigned char* data){
		data_ = std::make_shared<Data>(data);
	}

	const std::shared_ptr<Data> get()const {
		return data_;
	}

	bool isEmpty()const {
		return (data_->data == 0);
	}
private:

	std::shared_ptr<Data> data_;

};

/// <summary>
/// STBI_Resource
/// </summary>
class STBI_Resource {

public:

	STBI_Resource(const char* path, int nrChannels_need) :
		data_(stbi_load(path, &size_.x, &size_.y, &nrChannels_, nrChannels_need))
	{}
	STBI_Resource():
		nrChannels_(0),
		size_(0),
		data_(0)
	{}
	const glm::ivec2& getSize()const {
		return size_;
	}

	const STBI_Data& getData()const  {
		return data_;
	}

	int getNumChannels()const {
		return nrChannels_;
	}

	bool isEmtpy()const  {
		return data_.isEmpty();
	}

	GLenum getFormat()const {
		if (nrChannels_ == 3)		return  GL_RGB;
		else if (nrChannels_ == 4)	return  GL_RGBA;
		else						return GL_RED;
	}

	GLenum getInternalFormat(bool gamma)const {
		
		if (nrChannels_ == 3)		return gamma ? GL_SRGB : GL_RGB;
		else if (nrChannels_ == 4)	return  gamma ? GL_SRGB_ALPHA : GL_RGBA;
		else						return GL_RED;
	}

	GLFWimage getImage()const {
		GLFWimage result;
		result.height = size_.y;
		result.width = size_.x;
		result.pixels = data_.get()->data;
		return result;
	}
private:

	glm::ivec2 size_;
	int nrChannels_;
	STBI_Data data_;
};

/// <summary>
/// TextureResource
/// </summary>
class TextureResource:public Resource {
public:

	TextureResource(const glm::ivec2& size,unsigned nrChannels,bool gamma, const unsigned char * data) {
		size_ = size;
		if (nrChannels == 3) {
			internal_format_ = gamma ? GL_SRGB : GL_RGB;
			format_ = GL_RGB;
		}
		else if (nrChannels == 4) {
			internal_format_ = gamma ? GL_SRGB_ALPHA : GL_RGBA;
			format_ = GL_RGBA;
		}
		else format_ = internal_format_ = GL_RED;

		glTexture::bind2D(id_);
		glTexImage2D(GL_TEXTURE_2D, 0, internal_format_, size_.x, size_.y, 0, format_, GL_UNSIGNED_BYTE, data);
	}

	TextureResource(const STBI_Resource& resource,bool gamma):
	TextureResource(resource.getSize(),resource.getNumChannels(),gamma,resource.getData().get()->data)
	{}

	TextureResource():
		size_(0),
		id_(),
		internal_format_(GL_RED),
		format_(GL_RED)
	{}

	GLenum getFormat()const {
		return format_;
	}

	GLenum getInternalFormat()const {
		return internal_format_;
	}

	const glm::ivec2& getSize()const {
		return size_;
	}

	const TexturePointer& getId()const {
		return id_;
	}

private:

	glm::ivec2 size_;
	TexturePointer id_;
	GLenum internal_format_;
	GLenum format_;

};

/// <summary>
/// ImagerLoader
/// </summary>
class ImageLoader {

public:

	static const void flipVerticallyOnLoad(bool enable) {
		stbi_set_flip_vertically_on_load(enable);
	}

	static const TextureResource* getTexture(const std::string& path, bool gamma = 1, int nrChannels_need = 0) {
		auto resource = rTextures_.find(path);

		if (resource == rTextures_.end()) {
			try { loadTexture(path, gamma, nrChannels_need); }
			catch(const stbiResourceException& ex)
			{
				ex.log();
				return 0;
			}
			return &rTextures_[path];
		}

		return &resource->second;
	}

	static const STBI_Resource* getSTBI(const std::string& path, int nrChannels_need = 0) {
		auto resource = rSTBI.find(path);

		if (resource == rSTBI.end()) {
			try { loadSTBI(path, nrChannels_need); }
			catch (const stbiResourceException& ex)
			{
				ex.log();
				return 0;
			}
			return &rSTBI[path];
		}

		return &resource->second;
	}
	
private:
	static void loadTexture(const std::string& path,bool gamma, int nrChannels_need);
	static void loadSTBI(const std::string& path, int nrChannels_need = 0);

	static std::map<std::string, TextureResource> rTextures_;
	static std::map<std::string, STBI_Resource> rSTBI;

	ImageLoader() {}
	~ImageLoader() {}

};
#endif
