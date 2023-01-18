#ifndef IMAGE_LOADER_H
#define IMAGE_LOADER_H

#include"stb_image.h"
#include "Graphic/ContextTexture.h"
#include "System/Exception.h"
#include "Graphic/TextureEntity.h"

/// <summary>
/// STBI_Data
/// </summary>
class STBI_Data {

	struct Data {
		unsigned char* data;
		Data(unsigned char* data_) :data(data_) {}
		~Data() { stbi_image_free(data); }
	};

public:
	STBI_Data(unsigned char* data):data_(std::make_shared<Data>(data)) {}
	STBI_Data():data_(0) {}

	void set(unsigned char* data){ data_ = std::make_shared<Data>(data);}

	inline const std::shared_ptr<Data> get()const { return data_;}
	inline		unsigned char* data()		const {	return data_->data; }
	inline bool isEmpty()const { return (data_->data == 0);}
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
	
	inline GLenum format()const {
		if (nrChannels_ == 3)		return  GL_RGB;
		else if (nrChannels_ == 4)	return  GL_RGBA;
		else						return	GL_RED;
	}

	inline GLenum internalFormat(bool gamma)const {
		if (nrChannels_ == 3)		return gamma ? GL_SRGB : GL_RGB;
		else if (nrChannels_ == 4)	return  gamma ? GL_SRGB_ALPHA : GL_RGBA;
		else						return GL_RED;
	}

	inline GLFWimage getImage()const {
		//GLFWimage result = ;
		//result.width = size_.x;
		//result.height = size_.y;
		//result.pixels = data_.get()->data;
		return { size_.x ,size_.y ,data_.get()->data};
	}

	glm::ivec2 size_;
	int nrChannels_;
	STBI_Data data_;
private:
};

/// <summary>
/// TextureResource
/// </summary>
class ResourceTexture2D {
public:

	ResourceTexture2D(const glm::ivec2& size,unsigned nrChannels,bool gamma, const byte * data):
		format_(GL_RED, GL_RED),
		size_(size),
		sizeMipmaps_(0)
	{
		size_ = size;
		
		if (nrChannels == 3) {
			format_.internalFormat = gamma ? GL_SRGB : GL_RGB;
			format_.format = GL_RGB;
		}
		else if (nrChannels == 4) {
			format_.internalFormat = gamma ? GL_SRGB_ALPHA : GL_RGBA;
			format_.format = GL_RGBA;
		}
		glTexture::bind2D(id_);
		format_.dataImage2D(size_, data);
		setupPar();
	}
	
	ResourceTexture2D(const STBI_Resource& resource,bool gamma):
		ResourceTexture2D(resource.size_,resource.nrChannels_,gamma,resource.data_.get()->data)
	{}
	/*ResourceTexture2D():
		size_(0),
		format_(GL_RED, GL_RED),
		sizeMipmaps_(0)
	{
		glTexture::bind2D(id_);
		setupPar();
	}*/
	ResourceTexture2D(const glm::ivec2& size, const TextureFormatData& format, const void* data, int sizeMipmaps) :
		size_(size),
		format_(format),
		sizeMipmaps_(sizeMipmaps)
	{
		glTexture::bind2D(id_);
		format.dataImage2D(size_, data);
		setupPar();
		if (sizeMipmaps_ != 0) {
			if (sizeMipmaps_ != -1) glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, sizeMipmaps_);
			glGenerateMipmap(GL_TEXTURE_2D);
		}
		
	}

	TexPtr id_;
	glm::ivec2 size_;
	TextureFormatData format_;



	inline void filter(GLint filter_max, GLint filter_min) {
		if (filter_.max == filter_max && filter_.min == filter_min)return;
		glTexture::bind2D(id_);
		filter_.setup(GL_TEXTURE_2D, filter_max, filter_min);
	}
	inline void wrap(GLint S, GLint T) {
		if (wrap_.vec == glm::ivec2(S, T)) return;
		glTexture::bind2D(id_);
		wrap_.setup(GL_TEXTURE_2D, S, T);
	}
	inline const tFilter& getFilter()	const { return filter_; }
	inline const tWrap2D& getWrap()		const { return wrap_; }
	inline int sizeMipmaps()			const { return sizeMipmaps_; }

	inline void setMipmaps(int sizeMipmaps) {
		if (sizeMipmaps == sizeMipmaps_) return;
		glTexture::bind2D(id_);
		sizeMipmaps_ = sizeMipmaps;
		if (sizeMipmaps_ != 0) {
			if (sizeMipmaps_ != -1) glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, sizeMipmaps_);
			glGenerateMipmap(GL_TEXTURE_2D);
		}
	}
private:

	inline void setupPar() {
		wrap_.setup(GL_TEXTURE_2D);
		filter_.setup(GL_TEXTURE_2D);
	}

	tFilter filter_;
	tWrap2D wrap_;
	int sizeMipmaps_;
};

/// <summary>
/// ImagerLoader
/// </summary>
class ImageLoader {

public:
	static const void flipVerticallyOnLoad(bool enable) { stbi_set_flip_vertically_on_load(enable);}

	static std::shared_ptr <ResourceTexture2D> getTex2D(const std::string& path, bool gamma = 1, int nrChannels_need = 0) {
		auto resource = rTextures_.find(path);
		if (resource == rTextures_.end()) return loadTexture(path, gamma, nrChannels_need);;
		return resource->second;
	}

	static const STBI_Resource* getSTBI(const std::string& path, int nrChannels_need = 0) {
		auto resource = rSTBI.find(path);
		if (resource == rSTBI.end()) return loadSTBI(path, nrChannels_need) ? &rSTBI[path] : 0;
		return &resource->second;
	}
	
private:

	static std::shared_ptr <ResourceTexture2D> loadTexture(const std::string& path,bool gamma, int nrChannels_need);
	static bool loadSTBI(const std::string& path, int nrChannels_need = 0);

	static std::map<std::string, std::shared_ptr<ResourceTexture2D>> rTextures_;
	static std::map<std::string, STBI_Resource> rSTBI;

	ImageLoader() {}
	~ImageLoader() {}
};
#endif
