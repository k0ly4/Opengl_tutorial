
#ifndef MATRIX_H
#define MATRIX_H

#include "Transform.h"

/// <summary>
/// MatrixShell
/// </summary>
class Mat4Shell {

public:

    Mat4Shell() :
        modified(0),
        matrix_(1.f),
        inverse_(1.f) {}

    void set(const glm::mat4& matrix) {
        matrix_ = matrix;
        modified = 1;
    }

    const glm::mat4& mat4()const { return matrix_; }

    const glm::mat4& inverse()const {
        if (modified) {
            inverse_ = glm::inverse(matrix_);
            modified = 0;
        }
        return inverse_;
    }

private:
   
    mutable bool modified;
    mutable glm::mat4 inverse_;
    glm::mat4 matrix_;
   
};

/// <summary>
/// ProjectionMatrix
/// </summary>
class ProjMat{

public:
    ProjMat() {}
    inline const Mat4Shell& mat4Shell()const { return mat_; }

    inline const ProjData& data()const { return data_; }

    inline const glm::mat4& mat4()const { return mat_.mat4();}

    inline const glm::mat4& inverse()const { return mat_.inverse();}

    inline void set(float ratio_screen, float angle_fov, float _near, float _far) {
        data_.persp.near = _near;
        data_.persp.far = _far;
        data_.persp.ratio = ratio_screen;
        data_.persp.fov = angle_fov;
        set(data_.persp);
    }
    inline void set(const Perspective& proj) {
        data_.persp = proj;
        mat_.set(proj.getMatrix());
    }

    void set(const Box& box) {
        data_.ortho = box;
        mat_.set(box.getMatrix());
    }
    void set(const FloatRect& rect, float _near, float _far) {
        set(Box(rect, _near, _far));
    }

private:

    Mat4Shell mat_;
    ProjData data_;

};

/// <summary>
/// BasisMatrix
/// </summary>
class BasisMat {

public:

    BasisMat() { modified = 1; }

   inline const Mat4Shell& mat4Shell()const {
       solve();
       return matrix_;
   }

    inline const glm::mat4& mat4()const {
        solve();
        return matrix_.mat4();
    }

    inline const glm::mat4& inverse()const { return matrix_.inverse();}

    inline const Basis& basis()const { return basis_; }

    inline void setPosition(const glm::vec3& position) {
        if (basis_.position != position) {
            basis_.position = position;
            modified = 1;
        }
    }
    inline void set(const Basis& basis) {
        if (basis_ != basis) {
            basis_ = basis;
            modified = 1;
        }
    }

    inline void lookInDir(const glm::vec3& direction) {
        basis_.front = glm::normalize(-direction);
        modified = 1;
    }

private:

    mutable Mat4Shell matrix_;
    mutable bool modified;

    Basis basis_;

    inline void solve() const{
        if (modified) {
            matrix_.set(basis_.mat4());
            modified = 0;
        }
    }
};

#endif

