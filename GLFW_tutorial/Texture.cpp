#include "Texture.h"

    /// <summary>
    /// TextureCubeMap
    /// </summary>
    void TextureCubeMap::setParameteri() {
        glTexture::bindCubeMap(getID());
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, Wrap_S);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, Wrap_T);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, Wrap_R);

        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, Filter_Max);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, Filter_Min);
    }

    TextureCubeMap::TextureCubeMap() {
        wrap(GL_CLAMP_TO_EDGE);
        filter(GL_LINEAR);
        gen();
        setParameteri();
    }

    void TextureCubeMap::wrap(unsigned S, unsigned T, unsigned R) {
        Wrap_S = S;
        Wrap_T = T;
        Wrap_R = R;
        if (texture) setParameteri();
    }

    void TextureCubeMap::filter(unsigned MAG, unsigned MIN) {
        Filter_Min = MIN;
        Filter_Max = MAG;
        if (texture) setParameteri();
    }
  
    bool TextureCubeMap::load(const std::string& directory, bool flip_vertically, bool gamma) {
        stbi_set_flip_vertically_on_load(flip_vertically);
        std::string path[6] = {
           "right.jpg","left.jpg","top.jpg","bottom.jpg","front.jpg","back.jpg"
        };
        glTexture::bindCubeMap(getID());
        for (int i = 0; i < 6; i++) {
            GlImage data;
            if (FileManager::loadImage(&data, directory + path[i]))
                glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, data.internal_format(gamma), data.width, data.height, 0, data.format(), GL_UNSIGNED_BYTE, data.pixels);
            else return 0;
        }
        glTexture::bindCubeMap(0);
        return 1;
    }

    /// <summary>
    /// Texture2D
    /// </summary>
    void Texture2D::setParameteri() {
        glTexture::bind2D(getID());
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, Wrap_S);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, Wrap_T);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, Filter_Max);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, Filter_Min);
    }
    bool Texture2D::loadFromFile(const std::string& path_to_image, bool flip_vertically, bool generateMipmap, bool gamma) {
        stbi_set_flip_vertically_on_load(flip_vertically);
        if (FileManager::loadTexture(path_to_image, this, &size, gamma)) {
            setParameteri();
            if (generateMipmap) glGenerateMipmap(GL_TEXTURE_2D);
            return 1;
        }
        return 0;
    }
    void Texture2D::loadFromMemory(glm::uvec2 size, GLint internal_format, GLint format, const void* data, bool generateMipmap) {
        init();
        this->size = size;
        glTexImage2D(GL_TEXTURE_2D, 0, internal_format, size.x, size.y, 0, format,
            internal_format == GL_RGBA16F ? GL_FLOAT : GL_UNSIGNED_BYTE, data);
        if (generateMipmap)glGenerateMipmap(GL_TEXTURE_2D);
    }
    void Texture2D::create(size_t width, size_t height, GLint internal_format, GLint format) {
        init();
        size = glm::uvec2(width, height);
        GLenum type;
        if (internal_format == GL_RGBA16F || internal_format == GL_DEPTH_COMPONENT) type = GL_FLOAT;
        else type = GL_UNSIGNED_BYTE;

        glTexImage2D(GL_TEXTURE_2D, 0, internal_format, width, height, 0, format, type, NULL);
    }
    void Texture2D::wrap(GLint wrap_S, GLint wrap_T) {
        Wrap_S = wrap_S;
        Wrap_T = wrap_T;
        if (texture) {
            glTexture::bind2D(getID());
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, Wrap_S);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, Wrap_T);
        }
    }
    void Texture2D::filter(GLint filter_min, GLint filter_max) {
        Filter_Min = filter_min;
        Filter_Max = filter_max;

        if (texture) {
            glTexture::bind2D(getID());
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, Filter_Max);///GL_LINEAR
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, Filter_Min);//GL_LINEAR_MIPMAP_LINEAR
        }
    }

    /// <summary>
    /// TextureCubeDepth
    /// </summary>
    TextureCubeDepth::TextureCubeDepth(size_t width, size_t height) {
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