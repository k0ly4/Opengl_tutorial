#ifndef POST_EFFECTS_H
#define POST_EFFECTS_H

#include "Scene/PrimitiveEntity.h"
/// <summary>
/// Debugger
/// </summary>
class Debugger {
public:

    static void displayExposure(const Texture2D& texture, float exp) {
        const Shader& shader = glShader::get(glShader::frame_exposure);
        shader.use();
        shader.uniform("exposure", exp);
        texture.use(0);
        sBuffer::quad->getVAO().draw();
    }

    static void display(const Texture2D& texture) {
        const Shader& shader = glShader::get(glShader::frame);
        shader.use();
        texture.use(0);
        sBuffer::quad->getVAO().draw();
    }

    static void displayRedChannel(const Texture2D& texture) {
        const Shader& shader = glShader::get(glShader::red);
        shader.use();
        texture.use(0);
        sBuffer::quad->getVAO().draw();
    }

private:
    static float exposure;
};



/// <summary>
/// Filter
/// </summary>
class Filter {

public:

    void displayRed(const Texture2DLite& texture) {
        Red(glShader::get(glShader::red), texture);
    }
    void displayRed(const ArrayTexture2D& texture, int level) {
        Red(glShader::get(glShader::red_array), texture, level);
    }
    void setExposure(float exp) {
        exposure = exp;
    }
    void drawExposure(const Texture2DLite& texture) {
        Exposure(glShader::get(glShader::frame_exposure), texture);
    }

private:

        float exposure = 2.2f;

        void Exposure(const Shader& shader, const Texture2DLite& texture) {
            shader.use();
            shader.uniform("exposure", exposure);
            texture.use(0);
            sBuffer::quad->getVAO().draw();
        }
        void Red(const Shader& shader, const Texture2DLite& texture) {
            shader.use();
            texture.use(0);
            sBuffer::quad->getVAO().draw();
        }
        void Red(const Shader& shader, const ArrayTexture2D& texture, int level) {
            shader.use();
            texture.use(0);
            shader.uniform("level", level);
            sBuffer::quad->getVAO().draw();
        }
};
#endif