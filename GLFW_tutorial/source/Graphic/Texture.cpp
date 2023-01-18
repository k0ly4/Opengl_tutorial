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
                size_ = resource->size_;
            }
            else if (resource->size_ != size_) {
                printf("(!)Error::TextureCubeMap::STBI_Resource(path = \"%s\") have size not equal cube map size\n",path[i].c_str());
                return 0;
            }

            glTexImage2D(
                GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0,
                resource->internalFormat(gammaMod),
                size_.x,
                size_.y,
                0,
                resource->format(),
                GL_UNSIGNED_BYTE,
                resource->data_.data()
            );
        }

        glTexture::bindCubeMap(0);
        return 1;
    }

    void TextureCubeMap::create(const glm::ivec2& size, const TextureFormatData& format) {
        detach();
        glTexture::bindCubeMap(id_);
        size_ = size;
        for (unsigned int i = 0; i < 6; ++i){
            format.dataImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, size_, NULL);
        }
        glTexture::bindCubeMap(0);
    }

    /// <summary>
    /// Texture2D
    /// </summary>
    bool Texture2D::load(const std::string& path, int sizeMipmaps, bool gammaMod) {
        res_ = ImageLoader::getTex2D(path, gammaMod);
        if(res_ == 0) return 0;
        res_->setMipmaps(sizeMipmaps);
        return 1;
    }

    
    
    /// <summary>
    /// ArrayTexture2D
    /// </summary>
  
    void ArrayTexture2D::create(size_t count, const glm::ivec2& size, const TextureFormatData& format,const void* data){

        detach();
        size_ = size;
        count_ = count;
        format.dataImage3D(GL_TEXTURE_2D_ARRAY, size_, count, data);
    }