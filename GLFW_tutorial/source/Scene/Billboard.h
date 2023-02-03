#ifndef BILLBOARD_H
#define BILLBOARD_H
#include "PrimitiveEntity.h"
#include "Graphic/Geometry.h"
#include "Resource/ShaderArguments.h"

///Billboard------------------------------------------------
/// <summary>
/// 
/// </summary>
class Billboard :public Drawable
{
    
public:
    Billboard() :size_(1.f) {shaderHint = glShader::billboard;}

    inline void setPosition(const glm::vec3& new_pos, size_t index = 0) {
        if (mesh.vertices.size() <= index)  mesh.vertices.resize(index + 1);        
        else if (mesh[index].v1 == new_pos) return;

        mesh[index].v1 = new_pos;
        mesh.saveInBuffer();
    }

    inline void setSize(const glm::vec2& size) { size_ = size;}
    inline void setEye(const View3D* eye_) { eye = eye_; }
    inline void setTexture(const Texture2D& texture) {  texture_ = &texture;}
    inline void draw(const View* view, const Shader& shader) {
        shader.use();
        shader.uniform("VP",    view->VP());
        shader.uniform("right", eye->basis().right);
        shader.uniform("up",    eye->basis().up);
        shader.uniform("size",  size_);
        
        texture_->use(0);
        mesh.drawArrays(Render::POINTS);
    }
   
private:

    qGeometry <Vertex1<glm::vec3>> mesh;
    const Texture2D* texture_ =0 ;
    const View3D* eye = 0;
    glm::vec2 size_;

};
#endif
