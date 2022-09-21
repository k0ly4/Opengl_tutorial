#ifndef CAMERA_H
#define CAMERA_H

#include "Shader.h"
#include "Math.h"

class MatrixShell {
    bool needUpInverse;
    glm::mat4 matrix;
    glm::mat4 inverse;
public:
    MatrixShell() :needUpInverse(0), matrix(1.f) {}
    void set(const glm::mat4& matrix_){
        matrix = matrix_;
        needUpInverse = 1;
    }
    const glm::mat4& get()const {
        return matrix;
    }
    const glm::mat4& getInverse() {
        if (needUpInverse) {
            inverse = glm::inverse(matrix);
        }
        return inverse;
    }
};

class cMatrix {
    MatrixShell& shell;
public:
    cMatrix(MatrixShell& shell_):shell(shell_) {}
    const glm::mat4& get()const { 
        return shell.get(); 
    }
    const glm::mat4& inverse()const {
        return shell.getInverse();
    }
};
class ProjectionMatrix {
private:
    MatrixShell matrix;
    ProjData data;
public:
    ProjectionMatrix(){}
    inline cMatrix getMatrixShell() {
        return cMatrix(matrix);
    }

    inline const ProjData& getData()const {
        return data;
    }
    inline const glm::mat4& get()const {
        return matrix.get();
    }
    inline const glm::mat4& getInverse() {
        return matrix.getInverse();
    }

    void set(float ratio_screen, float angle_fov, float _near, float _far) {
        data.persp.near = _near;
        data.persp.far = _far;
        data.persp.ratio = ratio_screen;
        data.persp.fov = angle_fov;
        set(data.persp);
    }
    void set(const Perspective& proj) {
        data.persp = proj;
        matrix.set(proj.getMatrix());
    }

    void set(const Box& box) {
        data.ortho = box;
        matrix.set(box.getMatrix());
    }
    void set(const FloatRect& rect, float _near, float _far) {
        set(Box(rect, _near, _far));
    }
  
};
class BasisMatrix {
    MatrixShell matrix;
    bool needUpMatrix;
    Basis basis;
    inline void upMatrix() {
        if (needUpMatrix){
            matrix.set(basis.lookAt());   
        }
    }
public:
    BasisMatrix() {
        basis = { glm::vec3(1.f,0.f,0.f),GAME::WORLD_UP,glm::vec3(0.f,0.f,-1.f),glm::vec3(0.f) };
        needUpMatrix = 1;
    }

    inline cMatrix getMatrixShell() {
        upMatrix();
        return cMatrix(matrix);
    }
    inline const glm::mat4& get() {
        upMatrix();
        return matrix.get();
    }
    inline const glm::mat4& getInverse() {
        return matrix.getInverse();
    }
    inline const Basis& getBasis()const {
        return basis;
    }

    inline void setPosition(const glm::vec3& pos) {
        basis.position = pos;
        needUpMatrix = 1;
    }
    inline void look(const glm::vec3& direction) {
        basis.front = glm::normalize(-direction);
        needUpMatrix = 1;
    }
    void setBasis(const Basis& basis_) {
        basis = basis_;
    }
};

class View {
public:
    virtual  cMatrix getProjection() = 0;
    virtual  cMatrix getView() = 0;

    virtual glm::mat4 getVP() = 0;
    virtual glm::mat4 getInverseVP() = 0;
    virtual void use(const Shader&) = 0; 
};
class ViewMatrix :public View {
  
public:
    cMatrix getProjection() {
        return cMatrix(proj);
    }
    cMatrix getView() {
        return cMatrix(view);
    }
    glm::mat4 getVP() {
        return proj.get() * view.get();
    }
    glm::mat4 getInverseVP() {
        return view.getInverse() * proj.getInverse();
    }

    inline void use(const Shader& shader) {
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
class View2D:public View {
public:
   inline void setProjection(const FloatRect& rect) {
       proj.set(rect, -1.f, 1.f);
    }
   inline void use(const Shader& shader)const {
        shader.uniform("projection",proj.get());
    }
protected:
    ProjectionMatrix proj;
    BasisMatrix view;
};

class View3D :public View {

public:
    inline void orient(const View3D& _view) {
        proj = _view.proj;
        view = _view.view;
    }
  
    inline  glm::mat4 getInverseVP() {
        return view.getInverse() * proj.getInverse();
    }
    inline glm::mat4 getVP() {
        return proj.get() * view.get();
    }
    inline cMatrix getProjection() {
        return view.getMatrixShell();
    }
    inline cMatrix getView() {
         return view.getMatrixShell();
    }

    inline void toGlobal(glm::vec3& window_coord) {
        glm::vec4 res = view.getInverse() * proj.getInverse() * glm::vec4(window_coord, 1.f);
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

    inline void use(const Shader& shader) {
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
    inline BasisMatrix& getTransform() {
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
