#include "Texture.h"
#include "Graphic/ContextRender.h"
    /// <summary>
    /// TextureCubeMap
    /// </summary>
    /// 
    bool TextureCubeMap::loadFromDirectory(const std::string& directory, bool flipVertically, bool gammaMod) {

        ImageLoader::setFlipOnLoad(flipVertically);

        std::string path[6] = {
           "right.jpg","left.jpg","top.jpg","bottom.jpg","front.jpg","back.jpg"
        };
        bool size_init = 0;
        sTexture::bindCubeMap(id_);
        for (int i = 0; i < 6; i++) {
            auto resource = ImageLoader::getSTBI(directory + path[i]);
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

        sTexture::bindCubeMap(0);
        return 1;
    }

    void TextureCubeMap::create(const glm::ivec2& size, const TextureFormatData& format) {
        detach();
        sTexture::bindCubeMap(id_);
        size_ = size;
        for (size_t i = 0; i < 6; ++i){
            sTexture::dataImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, size_, format.internalFormat,format.format, nullptr);
        }
        sTexture::bindCubeMap(0);
    }

    void TextureCubeMap::bindToFramebuffer() { sRender::Framebuffer::bindTexture2D(GL_DEPTH_ATTACHMENT, id_); }
    void Texture2D::bindToFramebuffer(size_t unit) {
        sRender::Framebuffer::bindTexture2D(GL_COLOR_ATTACHMENT0 + unit, resource->id_);
    }
    /// <summary>
    /// ArrayTexture2D
    /// </summary>
  
    void ArrayTexture2D::create(size_t count, const glm::ivec2& size, const TextureFormatData& format,const void* data){

        detach();
        size_ = size;
        count_ = count;
        sTexture::dataImage3D(GL_TEXTURE_2D_ARRAY, size_, count, format.internalFormat, format.format, data);
    }