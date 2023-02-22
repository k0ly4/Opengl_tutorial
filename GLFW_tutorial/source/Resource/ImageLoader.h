#ifndef IMAGE_LOADER_H
#define IMAGE_LOADER_H

#include"stb_image.h"
#include "stb_image_write.h"


#include "Graphic/ContextTexture.h"
#include "System/Exception.h"
#include "Graphic/TextureEntity.h"

/// <summary>
/// STBI_Data
/// </summary>
class STBI_Data {

	struct Data {
		byte* data;
		Data(byte* data_) :data(data_) {}
		~Data() { stbi_image_free(data); }
	};

public:

	STBI_Data(unsigned char* data):data_(std::make_shared<Data>(data)) {}
	STBI_Data():data_(0) {}

	inline void set(byte* data) { data_ = std::make_shared<Data>(data); }

	//inline const std::shared_ptr<Data> get()const { return data_;}
	inline		unsigned char* data()		const {	return data_->data; }
	inline bool isEmpty()const { return (data_ == 0);}

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

	inline GLFWimage getImage()const { return { size_.x ,size_.y ,data_.data()};}
	inline bool isEmpty()const { return data_.isEmpty(); }

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

	ResourceTexture2D() {
	
	}

	ResourceTexture2D(const glm::ivec2& size, unsigned nrChannels, bool gamma, const void* data, int sizeMipmaps = 0):
		size_(size),
		sizeMips(sizeMipmaps)
	{
		format_.setByChannel(nrChannels, gamma);
		create(data);
	}
	ResourceTexture2D(const glm::ivec2& size, const TextureFormatData& format, const void* data, int sizeMipmaps = 0):
		size_(size),
		format_(format),
		sizeMips(sizeMipmaps)
	{
		create(data);
	}
	ResourceTexture2D(const STBI_Resource& resource,bool gamma):
		ResourceTexture2D(resource.size_,resource.nrChannels_, gamma, resource.data_.data())
	{}
	
	glm::ivec2 size_;
	
	inline void setFilter(const tFilter& filter) {
		if (filter_ == filter)return;
		filter_ = filter;
		sTexture::bind2D(id_);
		sTexture::parameter(GL_TEXTURE_2D, filter);
	}

	inline void setWrap(const tWrap2D& wrap) {
		if (wrap_ == wrap) return;
		wrap_ = wrap;
		sTexture::bind2D(id_);
		sTexture::parameter(GL_TEXTURE_2D, wrap_);
	}

	inline void genMipmaps(int sizeMipmaps) {
		if (sizeMipmaps == sizeMips) return;
		sizeMips = sizeMipmaps;
		sTexture::bind2D(id_);
		if (sizeMips != 0) sTexture::genMipmaps(GL_TEXTURE_2D, sizeMips);
	}

	inline const tFilter& getFilter()	const { return filter_; }
	inline const tWrap2D& getWrap()		const { return wrap_; }
	inline int	sizeMipmaps()			const { return sizeMips; }
	TextureId id_;

	inline void use(size_t text_unit)const noexcept {sTexture::bind2D(id_, GL_TEXTURE0 + text_unit);}

private:

	inline void create(const void* data) {

		sTexture::bind2D(id_);
		sTexture::dataImage2D(size_, format_, data);
		sTexture::parameter(GL_TEXTURE_2D, wrap_);
		sTexture::parameter(GL_TEXTURE_2D, filter_);
		if (sizeMips != 0) sTexture::genMipmaps(GL_TEXTURE_2D, sizeMips);
	}

	TextureFormatData format_;
	tFilter filter_;
	tWrap2D wrap_;
	int sizeMips;
	
};

/// <summary>
/// ImagerLoader
/// </summary>
class ImageLoader {

public:

	static const void setFlipOnLoad(bool enable) { stbi_set_flip_vertically_on_load(enable); }

	static std::shared_ptr <ResourceTexture2D> getTex2D(const std::string& path, bool hint_gamma = 1, int hint_nrChannels_need = 0) {
		return getTexture2D(path, hint_gamma, hint_nrChannels_need);
	}
	static std::shared_ptr <ResourceTexture2D> getTexture2D(const std::string& path, bool hint_gamma = 1, int hint_nrChannels_need = 0) {
		auto resource = resTextures.find(path);
		if (resource != resTextures.end()) return resource->second;
		std::shared_ptr<ResourceTexture2D> res = loadTexture2D(path, hint_gamma, hint_nrChannels_need);
		if (res) resTextures.insert({ path,res });
		return res;
	}
	static const std::shared_ptr<STBI_Resource> getSTBI(const std::string& path, int nrChannels_need = 0) {
		auto resource = resSTBI.find(path);
		if (resource != resSTBI.end()) return resource->second;
		std::shared_ptr<STBI_Resource> res = loadSTBI(path, nrChannels_need);
		if (res) resSTBI.insert({ path,res });
		return res;
	}
	//Без синхронизации со списком подходит для кратковременных текстур
	static std::shared_ptr <ResourceTexture2D>	loadTexture2D(const std::string& path, bool gamma =1, int nrChannels_need =0);
	//Без синхронизации со списком подходит для кратковременных картинок
	static std::shared_ptr<STBI_Resource>		loadSTBI(const std::string& path, int nrChannels_need = 0 );
	inline static int savePNG(const char* path, const glm::ivec2& size,int nrChannels_,const void* data) {
		return stbi_write_png(path, size.x, size.y, nrChannels_, data, size.x * nrChannels_);
	}
	inline static int savePNG(const std::string& path,const STBI_Resource& image) {
		return savePNG(path.c_str(), image.size_, image.nrChannels_, image.data_.data());
	}
	static int savePNG(const std::string& path, const ResourceTexture2D& image);
private:

	static std::map<std::string, std::shared_ptr<ResourceTexture2D>> resTextures;
	static std::map<std::string, std::shared_ptr<STBI_Resource>> resSTBI;

	ImageLoader() {}
	~ImageLoader() {}
};
#endif
