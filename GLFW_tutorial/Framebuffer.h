#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H


#include "Texture.h"
#include "buffer.h"
#include<map>
#include<random>
#include"Render.h"


class FrameBuffer :public GeneralRender {
    unsigned int depth;
    struct texture_data
    {
        GLint
            internal_format,
            format,
            filter;
        texture_data(GLint  text_internalformat, GLint  text_format, GLint text_filter) :
            internal_format(text_internalformat), format(text_format), filter(text_filter) {}
    };
    std::vector<texture_data> t_data;
    std::vector<GeneralTexture> color;
public:
    FrameBuffer() {
        glGenFramebuffers(1, &id);
        glGenRenderbuffers(1, &depth);
    }
    Texture2D getTexture(size_t index = 0) {
        return Texture2D(color[index], size);
    }
   void addTexture(GLint internalformat, GLint format, GLint filter) {
       t_data.push_back(texture_data(internalformat, format, filter));    
   }
    void implementDepth(unsigned int write_fbo) {
        glBindFramebuffer(GL_READ_FRAMEBUFFER, id);
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, write_fbo);
        glBlitFramebuffer(0, 0, size.x, size.y, 0, 0, size.x, size.y, GL_DEPTH_BUFFER_BIT, GL_NEAREST);
    }
    void implementDepth(FrameBuffer& write_fbo) {
        glBindFramebuffer(GL_READ_FRAMEBUFFER, id);
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, write_fbo.id);
        glBlitFramebuffer(0, 0, size.x, size.y, 0, 0, size.x, size.y, GL_DEPTH_BUFFER_BIT, GL_NEAREST);
    }
    void create(const glm::uvec2& size) {
        create(size.x, size.y);
    }
    void create(size_t width, size_t height)
    {
        size = glm::uvec2(width, height);
        std::vector<unsigned int> attachment(t_data.size());
        color.resize(t_data.size());

        glBindFramebuffer(GL_FRAMEBUFFER, id);
        for (size_t i = 0; i < color.size(); i++) {
          
            color[i].gen();
            glTexture::bind2D(color[i]);
          
            glTexImage2D(GL_TEXTURE_2D, 0, 
                t_data[i].internal_format, width, height, 0, t_data[i].format,
                t_data[i].internal_format == GL_RGBA16F ? GL_FLOAT : GL_UNSIGNED_BYTE, NULL);

            std::string ds = t_data[i].internal_format == GL_RGBA16F ? "Float" : "Unsigned BYTE";
            std::cout << "Framebuffer::texture::" << ds << std::endl;

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, t_data[i].filter);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, t_data[i].filter);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            
            attachment[i] = GL_COLOR_ATTACHMENT0 + i;
            glFramebufferTexture2D(GL_FRAMEBUFFER, attachment[i], GL_TEXTURE_2D, color[i].getID(), 0);
        }
        glDrawBuffers(attachment.size(), attachment.data());

        glBindRenderbuffer(GL_RENDERBUFFER, depth);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depth);

        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
            std::cout << "(!)ERROR::FRAMEBUFFER::NON-COMPLETE" << std::endl;
        }
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }
    ~FrameBuffer() {
        glDeleteRenderbuffers(1, &depth);
        glDeleteFramebuffers(1, &id);
    }
};

class RenderColor :public GeneralRender {
    struct texture_data
    {
        GLint
            internal_format,
            format,
            filter;
        texture_data(GLint  text_internalformat, GLint  text_format, GLint text_filter) :
            internal_format(text_internalformat), format(text_format), filter(text_filter) {}
    };
    std::vector<texture_data> t_data;
    std::vector<GeneralTexture> color;
public:
    RenderColor() {
        glGenFramebuffers(1, &id);
    }
    void addTexture(GLint internalformat, GLint format, GLint filter) {
        t_data.push_back(texture_data(internalformat, format, filter));
        color.push_back({});
        color.front().gen();
    }
    void create(const glm::uvec2& size_window) {
        create(size_window.x, size_window.y);
    }
    void create(size_t width, size_t height)
    {
        size = glm::uvec2( width,height);
        
        std::vector<unsigned int> attachment(t_data.size());

        GlRender::bind(*this);
        for (size_t i = 0; i < color.size(); i++) {
            glTexture::bind2D(color[i]);
         
            glTexImage2D(GL_TEXTURE_2D, 0, t_data[i].internal_format, width, height, 0, t_data[i].format, 
                t_data[i].internal_format == GL_RGBA16F ? GL_FLOAT : GL_UNSIGNED_BYTE,  NULL);

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, t_data[i].filter);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, t_data[i].filter);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);


            attachment[i] = GL_COLOR_ATTACHMENT0 + i;
            glFramebufferTexture2D(GL_FRAMEBUFFER, attachment[i], GL_TEXTURE_2D, color[i].getID(), 0);
        }
        glDrawBuffers(attachment.size(), attachment.data());
        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
            std::cout << "(!)ERROR::FRAMEBUFFER::NON-COMPLETE" << std::endl;
        }
        GlRender::unbind();
    }
    Texture2D getTexture(size_t index = 0) {
        return Texture2D(color[index],size);
    }
    ~RenderColor() {
        glDeleteFramebuffers(1, &id);
    }
};
class RenderTexture:public GeneralRender { 
    Texture2D texture;
public:
    RenderTexture() {
        glGenFramebuffers(1, &id);
    }
    void create(size_t width,size_t height, GLint format, GLint filterMin, GLint filterMax) {
        size = glm::uvec2(width, height);
        texture.filter(filterMin, filterMax);
        texture.wrap(GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE);
        
        texture.create(width, height, format, format);
        glBindFramebuffer(GL_FRAMEBUFFER, id);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture.getID(), 0);
        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
            std::cout << "(!)ERROR::FRAMEBUFFER::NON-COMPLETE" << std::endl;
        }
        GlRender::unbind();
        
    }
   inline Texture2D& getTexture() {
        return texture;
    }
    ~RenderTexture() {
        glDeleteFramebuffers(1, &id);
    }
};


//namespace effect {
//    class Filter {
//        std::vector<Shader> shader;
//        RenderColor pingPongFBO[2];
//        Primitive::Quad render_window;
//        enum filters :size_t
//        {
//            f_blur, f_add, f_exposure
//        };
//    public:
//        Filter(size_t width, size_t height) {
//            for (int i = 0; i < 2; i++) {
//                pingPongFBO[i].addTexture(GL_RGBA16F, GL_RGBA, GL_LINEAR);
//                pingPongFBO[i].create(width, height);
//            }
//            shader.push_back(Shader("shaders\\texture\\general\\framebuffer\\blur\\shader.vs", "shaders\\texture\\general\\framebuffer\\blur\\shader.fs"));
//            shader[f_blur].use();
//            shader[f_blur].uniform("image", 0);
//
//            shader.push_back(Shader("shaders\\texture\\general\\framebuffer\\mix\\shader.vs", "shaders\\texture\\general\\framebuffer\\mix\\shader.fs"));
//            shader[f_add].use();
//            shader[f_add].uniform("image1", 0);
//            shader[f_add].uniform("image2", 1);
//
//            shader.push_back(Shader("shaders\\texture\\general\\framebuffer\\exposure\\shader.vs", "shaders\\texture\\general\\framebuffer\\exposure\\shader.fs"));
//            shader[f_exposure].use();
//            shader[f_exposure].uniform("image", 0);
//
//        }
//        Texture2D blur(Texture2D image, size_t amount) {
//            shader[f_blur].use();
//            image.use(0);
//            bool horizontal = 1;
//            for (size_t i = 0; i < amount; i++) {
//                FrameBuffer::use(pingPongFBO[horizontal]);
//                shader[f_blur].uniform("horizontal", horizontal);
//                render_window.draw();
//                horizontal = !horizontal;
//                pingPongFBO[horizontal].getTexture().use(0);
//            }
//            FrameBuffer::use(0);
//            return pingPongFBO[!horizontal].getTexture();
//        }
//        void draw_mix_add(Texture2D image1, Texture2D image2, float exposure) {
//            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//            shader[f_add].use();
//            shader[f_add].uniform("exposure", exposure);
//            image1.use(0);
//            image2.use(1);
//            render_window.draw();
//        }
//        void draw_exposure(Texture2D image, float exposure) {
//            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//            shader[f_exposure].use();
//            shader[f_exposure].uniform("exposure", exposure);
//            image.use(0);
//            render_window.draw();
//        }
//    };
//}
//
//class SSAO {
//    Shader shader, shader_dem, shader_blur;
//    Texture2D noise_tex;
//    float lerp(float a, float b, float c) {
//        return a + c * (b - a);
//    }
//    RenderColor fbo;
//    Primitive::Quad render_window;
//    void blur() {
//        shader_blur.use();
//        fbo.getTexture().use(0);
//        render_window.draw();
//        FrameBuffer::use(0);
//    }
//public:
//    SSAO(glm::mat4 &projection) {
//        shader.load("shaders\\advanced_light\\ssao\\shader.vs", "shaders\\advanced_light\\ssao\\shader.fs");
//        shader.use();
//        shader.uniform("gPosition", 0);
//        shader.uniform("gNormal", 1);
//        shader.uniform("texNoise", 2);
//        shader.uniform("radius", 0.5f);
//        shader.uniform("projection", projection);
//
//        shader_dem.load("shaders\\advanced_light\\ssao\\test\\shader.vs", "shaders\\advanced_light\\ssao\\test\\shader.fs");
//        shader_dem.use();
//        shader_dem.uniform("image", 0);
//
//        shader_blur.load("shaders\\advanced_light\\ssao\\blur\\shader.vs", "shaders\\advanced_light\\ssao\\blur\\shader.fs");
//        shader_blur.use();
//        shader_blur.uniform("image", 0);
//    }
//
//    void create(const glm::uvec2& size_window, size_t amount_samples, size_t size_noise) {
//        std::uniform_real_distribution<GLfloat> randomFloat(0.f, 1.f);
//        std::default_random_engine generator;
//        std::vector<glm::vec3> kernel(amount_samples);
//
//        for (size_t i = 0; i < amount_samples; i++) {
//            kernel[i].x = randomFloat(generator) * 2.f - 1.f;
//            kernel[i].y = randomFloat(generator) * 2.f - 1.f;
//            kernel[i].z = randomFloat(generator);
//            kernel[i] = glm::normalize(kernel[i]);
//            float scale = float(i) / amount_samples;
//            scale = lerp(0.1f, 1.f, scale * scale);
//            kernel[i] *= randomFloat(generator) * scale;
//        }
//        std::vector<glm::vec3>noise(size_noise * size_noise);
//        for (size_t i = 0; i < noise.size(); i++) {
//            noise[i].x = randomFloat(generator) * 2.f - 1.f;
//            noise[i].y = randomFloat(generator) * 2.f - 1.f;
//            noise[i].z = 0.f;
//        }
//        noise_tex.loadFromMemory(glm::uvec2(size_noise, size_noise), GL_RGBA16F,GL_RGB, noise.data(),0);
//        noise_tex.filter(GL_NEAREST, GL_NEAREST);
//        noise_tex.wrap(GL_REPEAT, GL_REPEAT);
//
//        fbo.addTexture(GL_RED, GL_RED, GL_NEAREST);
//        fbo.create(size_window);
//
//        shader.use();
//        shader.uniform("samples", kernel);
//        shader.uniform("noiseScale", glm::vec2((float)size_window.x / size_noise, (float)size_window.y / size_noise));
//    }
//    Texture2D getTexture() {
//        return fbo.getTexture();
//    }
//    void use(RenderBuffer& gBuffer) {
//        FrameBuffer::use(fbo);
//        glClear(GL_COLOR_BUFFER_BIT);
//        shader.use();
//        gBuffer.getTexture(0).use(0);
//        gBuffer.getTexture(1).use(1);
//        noise_tex.use(2);
//        render_window.draw();
//        blur();
//    }
//    void display() {
//
//        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//        shader_dem.use();
//        fbo.getTexture().use(0);
//        render_window.draw();
//    }
//};
#endif