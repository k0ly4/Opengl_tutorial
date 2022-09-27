#ifndef DEPTH_FRAMEBUFFER_H
#define DEPTH_FRAMEBUFFER_H
#include "Framebuffer.h"


/// <summary>
/// RenderTextureDepth
/// </summary>
class RenderTextureDepth :public GeneralRender, public RenderTarget {
    
public:

    RenderTextureDepth() {
        glGenFramebuffers(1, &id_);
    }

    void create(const glm::ivec2& Size);

    void create(int width, int height) {
        create(glm::ivec2(width, height));
    }

    Texture2D getTexture() {
        return Texture2D(texture, size_);
    }

private:
    TexturePointer texture;
};

/// <summary>
/// RenderCascadedDepth
/// </summary>
class  RenderCascadedDepth :public GeneralRender, public RenderTarget {
    
public:

    RenderCascadedDepth() {
        glGenFramebuffers(1, &id_);
    }

    bool create(int cascadeLevels, const glm::ivec2& Size);

    void bind() {
        GlRender::bind(*this);
    }

    void create(size_t cascadeLevels, int width, int height) {
        create(cascadeLevels, glm::ivec2(width, height));
    }

    const ArrayTexture2D& getTexture()const {
        return texture;
    }

private:
    ArrayTexture2D texture;
};

#endif