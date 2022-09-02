#pragma once
#ifndef CAMERA_H
#define CAMERA_H

#include "Shader.h"
struct Basis {
    glm::vec3 
        right,
        up,
        front;
    glm::vec3 position;
    inline glm::mat4 lookAt()const  {
        return glm::lookAt(position, position + front, GAME::WORLD_UP);
    }
};
struct Perspective{
    float near;
    float far;
    float ratio;
    float fov;
};
class View {
protected:
    class Projection {  
        union ProjData
        {
            Box ortho;
            Perspective persp;
            ProjData() {

                    std::memset(this, 0, sizeof(ProjData)); 
            }
        };
        glm::mat4 inverse;
        glm::mat4 matrix;
        bool need_up_inverse=0;
        ProjData data;
        friend class View;
        friend class Camera;
        friend class View3D;
        inline void set(const glm::mat4& _matrix) {
            matrix = _matrix;
            need_up_inverse = 1;
        }
    public:
        Projection(): matrix(1.f) {}
        inline const ProjData& Setup()const {
            return data;
        }
        const glm::mat4& Matrix()const {
            return matrix;
        }
        const glm::mat4& Inverse() {
            if (need_up_inverse) {
                inverse = glm::inverse(matrix);
                need_up_inverse = 0;
            }
            return inverse;
        }
        void set(const FloatRect& rect, float _near, float _far) {
            data.ortho = Box(rect, _near, _far);
            set(glm::ortho(rect.x, rect.w, rect.h, rect.y, _near, _far));
        }
        void set(const Box& box) {
            data.ortho = box;  
            set(box.ortho());
        }
        void set(float ratio_screen, float angle_fov, float _near, float _far) {
            data.persp.near = _near;
            data.persp.far = _far;
           
            data.persp.ratio = ratio_screen;
            data.persp.fov = angle_fov;
            set(glm::perspective(glm::radians(angle_fov), ratio_screen, _near, _far));
        }
    };
    class MatrixView {
        glm::mat4 matrix;
        glm::mat4 inverse;
        bool need_up_inverse = 0;
        bool need_up_matrix = 0;
        Basis basis;
        inline void upMatrix(){
            matrix = basis.lookAt();
            need_up_matrix = 0;
            need_up_inverse = 1;
        }
        inline void upInverse() {
            inverse = glm::inverse(matrix);
            need_up_inverse = 0;
        }
        friend class View;
        friend class Camera;
        friend class View3D;
    public:
        MatrixView() :matrix(1.f){
         basis = { glm::vec3(1.f,0.f,0.f),GAME::WORLD_UP,glm::vec3(0.f,0.f,-1.f),glm::vec3(0.f)};
        }
        inline const glm::mat4& Matrix() {
            if (need_up_matrix) upMatrix();
            return matrix;
        }
        inline void setPosition(const glm::vec3& pos) {
            basis.position = pos;
            need_up_matrix = 1;
        }
        inline const glm::mat4& Inverse() {
            if (need_up_inverse) upInverse();
            return inverse;
        }
        inline const Basis& Basis()const {
            return basis;
        }
        inline void look(const glm::vec3& direction) {
            basis.front = glm::normalize(-direction);
            need_up_matrix = 1;
        }
    };   

    Projection proj;
    MatrixView view;
public:
    inline Projection& getProjection(){
        return proj;
    }
    inline Projection& Projection() {
        return proj;
    }
    inline MatrixView& getMatrixView() {
        return view;
    } 
    void setPosition(const glm::vec3& position) {
        view.setPosition(position);
    }
    void setPosition(float x,float y, float z) {
        view.setPosition(glm::vec3(x,y,z));
    }
    const glm::vec3& getPosition() {
        return view.basis.position;
    }
    inline MatrixView& Transform() {
        return view;
    }
    inline void orient(const View& _view) {
        proj = _view.proj;
        view = _view.view;
    }
    inline glm::mat4 getVP() {
        return proj.Matrix() * view.Matrix();
    }
    inline glm::mat4 getInverseVP() {
        return view.Inverse() * proj.Inverse();
    }
    inline void toGlobal(glm::vec3& window_coord) {
        glm::vec4 res = view.Inverse() * proj.Inverse()* glm::vec4(window_coord, 1.f);
        window_coord = res / res.w;
    }
   
    virtual void use(const Shader&) = 0; 
};
class View2D:public View {
public:
   inline void setProjection(const FloatRect& rect) {
       proj.set(rect, -1.f, 1.f);
    }
   inline void use(const Shader& shader)const {
        shader.uniform("projection",proj.Matrix());
    }
};
class View3D :public View { 
public:
    inline void setProjection(const FloatRect& rect,float near,float far) {
        proj.set(rect, near, far);
    }
   
    
    inline void setProjection(float ratio_screen, float angle_fov = 45.f, float near = 0.1f, float far = 150.f) { 
        proj.set(ratio_screen, angle_fov, near, far);
    }
    inline void setProjection(const Box& box) {
        proj.set(box);
    }
    void use(const Shader& shader) {
        shader.uniform("projection", proj.Matrix());
        shader.uniform("camera", view.Matrix());
    }
};
class Camera:public View3D { 
    struct AngleEuler {
        float yaw;
        float pitch;
        void cutPitch(float angle) {
            if (pitch > angle) pitch = angle;
            else if (pitch < -angle)pitch = -angle;
        }
        void getBasis(Basis& basis) {
            basis.front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
            basis.front.y = sin(glm::radians(pitch));
            basis.front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
        }
        AngleEuler() :yaw(0.f), pitch(0.f) {}
    }euler;
    float  SENSITIVITY = 0.05f;
public:
    glm::vec2 cur_pos_mouse;
    Camera(): cur_pos_mouse(0.f)
    {}
    inline void move(const glm::vec3& vec) {
        view.basis.position += vec.z * view.basis.front;
        view.basis.position = view.basis.position + vec.x * glm::normalize(glm::cross(view.basis.front,GAME::WORLD_UP));
        view.need_up_matrix = 1;
    } 
    void mouse_move(glm::vec2 pos_mouse) {
        glm::vec2 offset = pos_mouse - cur_pos_mouse;
        cur_pos_mouse = pos_mouse;

        euler.yaw += offset.x * SENSITIVITY;
        euler.pitch += -offset.y * SENSITIVITY;

        euler.cutPitch(89.f);
        euler.getBasis(view.basis);

        view.basis.front = glm::normalize(view.basis.front);
        view.basis.right = glm::normalize(glm::cross(view.basis.front, GAME::WORLD_UP));
        view.basis.up =    glm::normalize(glm::cross(view.basis.right, view.basis.front));
        view.need_up_matrix = 1;
    } 
};

#endif
