#ifndef CAMERA_H
#define CAMERA_H

#include "Shader.h"
#include "Math.h"

/// View------------------------------------------------------------
/// <summary>
/// Interface
/// </summary>
class View {
public:
    virtual const MatrixShell& getProjection() const = 0;
    virtual const MatrixShell& getView()const = 0;

    virtual glm::mat4 getVP()const = 0;
    virtual glm::mat4 getInverseVP()const = 0;
    virtual void use(const Shader&) const= 0; 
};

/// ViewMatrix------------------------------------------------------------
/// <summary>
/// 
/// </summary>
class ViewMatrix :public View {
  
public:

    inline const MatrixShell& getProjection()const {
        return proj;
    }
    inline const MatrixShell& getView()const {
        return view;
    }
    inline glm::mat4 getVP()const {
        return proj.get() * view.get();
    }
    inline glm::mat4 getInverseVP()const {
        return view.getInverse() * proj.getInverse();
    }

    inline void use(const Shader& shader) const{
        shader.uniform("projection", proj.get());
        shader.uniform("camera", view.get());
    }

    void setProjection(const glm::mat4& matrix) {
        proj.set(matrix);
    }

    void setView(const glm::mat4& matrix) {
        view.set(matrix);
    }
private:

    MatrixShell view;
    MatrixShell proj;
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
   inline void setProjection(const FloatRect& rect) {
       proj.set(rect, -1.f, 1.f);
   }
   inline void setProjection(const Box& box) {
       proj.set(box);
   }
   ///override
   inline void use(const Shader& shader)const override {
        shader.uniform("projection",proj.get());
        shader.uniform("camera", glm::mat4(1.f));
   }
   inline  glm::mat4 getInverseVP()const override {
       return view.getInverse() * proj.getInverse();
   }
   inline glm::mat4 getVP()const override {
       return proj.get() * view.get();
   }
   inline const MatrixShell& getProjection()const override {
       return view.getMatrixShell();
   }
   inline const MatrixShell& getView() const override {
       return view.getMatrixShell();
   }

protected:

    ProjectionMatrix proj;
    BasisMatrix view;

};

/// View3D------------------------------------------------------------
/// <summary>
/// 
/// </summary>
class View3D :public View {

public:
    inline void orient(const View3D& _view) {
        proj = _view.proj;
        view = _view.view;
    }
  
    inline  glm::mat4 getInverseVP()const {
        return view.getInverse() * proj.getInverse();
    }
    inline glm::mat4 getVP()const {
        return proj.get() * view.get();
    }

    inline const MatrixShell& getProjection()const {
        return view.getMatrixShell();
    }
    inline const MatrixShell& getView() const {
         return view.getMatrixShell();
    }

    inline void toGlobal(glm::vec3& window_coord)const {
        glm::vec4 res = getInverseVP() * glm::vec4(window_coord, 1.f);
        window_coord = res / res.w;
    }

    inline void setProjection(const FloatRect& rect,float near,float far) {
        proj.set(rect, near, far);
    }
    inline void setProjection(float ratio_screen, float angle_fov = 45.f, float near = 0.1f, float far = 150.f) { 
        proj.set(ratio_screen, angle_fov, near, far);
    }
    inline void setProjection(const Perspective& persp) {
        proj.set(persp);
    }
    inline void setProjection(const Box& box) {
        proj.set(box);
    }

    inline void use(const Shader& shader)const {
        shader.uniform("projection", proj.get());
        shader.uniform("camera", view.get());
    }

    inline void setPosition(const glm::vec3& position) {
        view.setPosition(position);
    }
    inline void setPosition(float x, float y, float z) {
        view.setPosition(glm::vec3(x, y, z));
    }
    inline const glm::vec3& getPosition()const {
        return view.getBasis().position;
    }

    inline const BasisMatrix& getTransform()const {
        return view;
    }
    inline const ProjData& getProjectionData()const {
        return proj.getData();
    }
    inline const Basis& getBasis()const {
        return view.getBasis();
    }
protected:

    ProjectionMatrix proj;
    BasisMatrix view;
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
