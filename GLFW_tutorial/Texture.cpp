#include "Texture.h"

    /// <summary>
    /// TextureCubeMap
    /// </summary>
    /// 

    TextureCubeMap::TextureCubeMap() {
        wrap(TextureWrap::ClampToEdge);
        filter(TextureFilter::Linear, TextureFilter::Linear);
    }
  
    bool TextureCubeMap::loadFromDirectory(const std::string& directory, bool flipVertically, bool gammaMod) {

        ImageLoader::flipVerticallyOnLoad(flipVertically);

        std::string path[6] = {
           "right.jpg","left.jpg","top.jpg","bottom.jpg","front.jpg","back.jpg"
        };

        glTexture::bindCubeMap(id_);
        for (int i = 0; i < 6; i++) {
            const STBI_Resource* resource = ImageLoader::getSTBI(directory + path[i]);
            if (resource == 0)  return 0;

            glTexImage2D(
                GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, 
                resource->getInternalFormat(gammaMod), 
                resource->getSize().x,
                resource->getSize().y,
                0, 
                resource->getFormat(),
                GL_UNSIGNED_BYTE, 
                resource->getData().get()->data
            );
        }

        glTexture::bindCubeMap(0);
        return 1;
    }

    /// <summary>
    /// Texture2D
    /// </summary>
    bool Texture2D::loadFromFile(const std::string& path, bool generateMipmap, bool gammaMod) {
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

    void Texture2D::loadFromMemory(const glm::ivec2& size, GLint internal_format, GLint format, const void* data, bool generateMipmap) {
        detach();
        size_ = size;
        isGenerateMipmap_ = generateMipmap;
        glTexImage2D(GL_TEXTURE_2D, 0, internal_format, size.x, size.y, 0, format,
            internal_format == GL_RGBA16F ? GL_FLOAT : GL_UNSIGNED_BYTE, data);

        if (isGenerateMipmap_)
            glGenerateMipmap(GL_TEXTURE_2D);
    }

    void Texture2D::create(int width, int height, GLint internal_format, GLint format) {
        detach();
        size_ = glm::uvec2(width, height);
        GLenum type;
        if (internal_format == GL_RGBA16F || internal_format == GL_DEPTH_COMPONENT) type = GL_FLOAT;
        else type = GL_UNSIGNED_BYTE;

        glTexImage2D(GL_TEXTURE_2D, 0, internal_format, width, height, 0, format, type, NULL);
    }

    /// <summary>
    /// TextureCubeDepth
    /// </summary>
    TextureCubeDepth::TextureCubeDepth(int width, int height) {
        glGenTextures(1, &map);
        glBindTexture(GL_TEXTURE_CUBE_MAP, map);
        for (size_t i = 0; i < 6; i++) {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
        }
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        /*   float color_border[] = { 1.f,1.f,1.f,1.f };
           glTexParameterfv(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_BORDER_COLOR, color_border);*/
        this->height = height;
        this->width = width;
    }
    void TextureCubeDepth::bind(unsigned int fbo) {
        glBindFramebuffer(GL_FRAMEBUFFER, fbo);
        glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, map, 0);
        glDrawBuffer(GL_NONE);
        glReadBuffer(GL_NONE);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }