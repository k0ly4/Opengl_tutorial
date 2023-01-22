#ifndef CAMERA_H
#define CAMERA_H

#include "Resource/Shader.h"
#include "Math/Math.h"

/// View------------------------------------------------------------
/// <summary>
/// Interface
/// </summary>
class View {
public:
    virtual const Mat4Shell& proj() const = 0;
    virtual const Mat4Shell& view()const = 0;

    virtual glm::mat4 VP()const = 0;
    virtual glm::mat4 inverseVP()const = 0;
    virtual void use(const Shader&) const= 0; 
    inline void useVP(const Shader& shader) const { shader.uniform("VP", VP());}
};

/// ViewMatrix------------------------------------------------------------
/// <summary>
/// 
/// </summary>
class ViewMatrix :public View {
  
public:
    ViewMatrix() {}
    ViewMatrix(const glm::mat4& proj,const glm::mat4& view)
    {
        setProjection(proj);
        setView(view);
    }
    inline const Mat4Shell& proj()const override { return proj_; }
    inline const Mat4Shell& view()const override {       return view_; }
    inline glm::mat4 VP()const {                return proj_.mat4() * view_.mat4();}
    inline glm::mat4 inverseVP()const {         return view_.inverse() * proj_.inverse();}

    inline void use(const Shader& shader) const{
        shader.uniform("projection",    proj_.mat4());
        shader.uniform("camera",        view_.mat4());
    }
    
    inline void setProjection(const glm::mat4& matrix) { proj_.set(matrix);}
    inline void setView(const glm::mat4& matrix) { view_.set(matrix); }

private:

    Mat4Shell view_;
    Mat4Shell proj_;
};

/// View2D------------------------------------------------------------
/// <summary>
/// View for 2D world coord
/// </summary>
class View2D:public View {
public:
    View2D()
    {
    }
   inline void setProjection(const FloatRect& rect) { proj_.set(rect, -1.f, 1.f);}
   inline void setProjection(const Box& box) { proj_.set(box); }
   ///override
   inline void use(const Shader& shader)const override {
        shader.uniform("projection", proj_.mat4());
        shader.uniform("camera", glm::mat4(1.f));
   }
   inline glm::mat4 inverseVP()         const override { return view_.inverse() * proj_.inverse(); }
   inline glm::mat4 VP()                const override { return proj_.mat4() * view_.mat4(); }
   inline const Mat4Shell& proj() const override { return proj_.mat4Shell();}
   inline const Mat4Shell& view()       const override { return view_.mat4Shell();}

protected:

    ProjMat proj_;
    BasisMat view_;

};

/// View3D------------------------------------------------------------
/// <summary>
/// 
/// </summary>
class View3D :public View {

public:
    inline void orient(const View3D& _view) {
        proj_ = _view.proj_;
        view_ = _view.view_;
    }
  
    inline glm::mat4 inverseVP()const           {           return view_.inverse() * proj_.inverse(); }
    inline glm::mat4 VP()const                  {           return proj_.mat4() * view_.mat4();}

    inline const Mat4Shell& proj() const override {         return proj_.mat4Shell(); }
    inline const Mat4Shell& view()       const override {   return view_.mat4Shell(); }

    inline void toGlobal(glm::vec3& window_coord)const {
        glm::vec4 res = inverseVP() * glm::vec4(window_coord, 1.f);
        window_coord = res / res.w;
    }

    inline void setProjection(const FloatRect& rect,float near,float far) { proj_.set(rect, near, far);}
    inline void setProjection(float ratio_screen, float angle_fov = 45.f, float near = 0.1f, float far = 150.f) {
        proj_.set(ratio_screen, angle_fov, near, far);
    }
    inline void setProjection(const Perspective& persp) {   proj_.set(persp); }
    inline void setProjection(const Box& box) {             proj_.set(box);}

    inline void use(const Shader& shader)const {
        shader.uniform("projection",    proj_.mat4());
        shader.uniform("camera",        view_.mat4());
    }

    inline void setPosition(const glm::vec3& position) {view_.setPosition(position); }

    inline void setPosition(float x, float y, float z) {view_.setPosition(glm::vec3(x, y, z));}

    inline const glm::vec3& position()const     {return view_.basis().position; }

    inline const BasisMat& basisMat()const      {return view_;}

    inline const ProjData& projData()const      {return proj_.data();}

    inline const Basis& basis()const            {return view_.basis();}

    inline const float renderDistance()const    {return proj_.data().persp.far;}

protected:

    ProjMat proj_;
    BasisMat view_;
};

/// Camera------------------------------------------------------------
/// <summary>
///
/// </summary>
class Camera :public View3D {

public:
    glm::vec2 cur_pos_mouse;
    Camera() : cur_pos_mouse(0.f), angle(0.f,0.f)
    {}
    void move(const glm::vec3& vec);
    void mouse_move(glm::vec2 pos_mouse);

private:

    AngleEuler angle;
    float  SENSITIVITY = 0.05f;
};

#endif
