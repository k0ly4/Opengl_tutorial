#ifndef FILTER_H
#define FILTER_H

#include "PrimitiveEntity.h"

class Filter {

public:

    void displayRed(const Texture2D& texture) {
        Red(glShader::get(glShader::red), texture);
    }
    void displayRed(const ArrayTexture2D& texture, int level) {
        Red(glShader::get(glShader::red_array), texture, level);
    }
    void setExposure(float exp) {
        exposure = exp;
    }
    void drawExposure(const Texture2D& texture) {
        Exposure(glShader::get(glShader::frame_exposure), texture);
    }

private:

        float exposure = 2.2f;

        void Exposure(const Shader& shader, const Texture2D& texture) {
            shader.use();
            shader.uniform("exposure", exposure);
            texture.use(0);
            sBuffer::quad.getVAO().draw();
        }
        void Red(const Shader& shader, const Texture2D& texture) {
            shader.use();
            texture.use(0);
            sBuffer::quad.getVAO().draw();
        }
        void Red(const Shader& shader, const ArrayTexture2D& texture, int level) {
            shader.use();
            texture.use(0);
            shader.uniform("level", level);
            sBuffer::quad.getVAO().draw();
        }
};
#endif