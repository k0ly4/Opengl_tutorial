#include "Texture.h"

    /// <summary>
    /// TextureCubeMap
    /// </summary>
    /// 
    bool TextureCubeMap::loadFromDirectory(const std::string& directory, bool flipVertically, bool gammaMod) {

        ImageLoader::flipVerticallyOnLoad(flipVertically);

        std::string path[6] = {
           "right.jpg","left.jpg","top.jpg","bottom.jpg","front.jpg","back.jpg"
        };
        bool size_init = 0;
        glTexture::bindCubeMap(id_);
        for (int i = 0; i < 6; i++) {
            const STBI_Resource* resource = ImageLoader::getSTBI(directory + path[i]);
            if (resource == 0)  return 0;

            if (size_init == 0) {
                size_ = resource->getSize();
            }
            else if (resource->getSize()!= size_) {
                printf("(!)Error::TextureCubeMap::STBI_Resource(path = \"%s\") have size not equal cube map size\n",path[i].c_str());
                return 0;
            }

            glTexImage2D(
                GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, 
                resource->getInternalFormat(gammaMod), 
                size_.x,
                size_.y,
                0, 
                resource->getFormat(),
                GL_UNSIGNED_BYTE, 
                resource->getData().get()->data
            );
        }

        glTexture::bindCubeMap(0);
        return 1;
    }

    void TextureCubeMap::create(const glm::ivec2& size, const TextureFormat& format) {
        detach();
        glTexture::bindCubeMap(id_);
        size_ = size;
        for (unsigned int i = 0; i < 6; ++i){
            format.TexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, size_, NULL);
        }
        glTexture::bindCubeMap(0);
    }

    /// <summary>
    /// Texture2D
    /// </summary>
    bool Texture2D::getPath(const std::string& path, bool generateMipmap, bool gammaMod) {
        const TextureResource* resource = ImageLoader::getTexture(path, gammaMod);
        if(resource == 0) return 0;

        id_ = resource->getId();
        size_ = resource->getSize();
        isGenerateMipmap_ = generateMipmap;

        glTexture::bind2D(id_);
        wrap_.setup(GL_TEXTURE_2D);
        filter_.setup(GL_TEXTURE_2D);
        if (isGenerateMipmap_) {
            glGenerateMipmap(GL_TEXTURE_2D);
        }
        return 1;
    }

    void Texture2D::create(const glm::ivec2& size, const TextureFormat& format, const void* data, bool generateMipmap) {

        detach();
        size_ = size;
        isGenerateMipmap_ = generateMipmap;
        format.setDataTexture2D(size, data);

        if (isGenerateMipmap_)
            glGenerateMipmap(GL_TEXTURE_2D);
    }
    
    /// <summary>
    /// ArrayTexture2D
    /// </summary>
  
    void ArrayTexture2D::create(size_t count, const glm::ivec2& size, const TextureFormat& format,const void* data){

        detach();
        size_ = size;
        count_ = count;
        format.TexImage3D(GL_TEXTURE_2D_ARRAY, size_, count, data);
    }