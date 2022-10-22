
#ifndef MATRIX_H
#define MATRIX_H

#include "Transform.h"

/// <summary>
/// MatrixShell
/// </summary>
class MatrixShell {

public:

    MatrixShell() :
        needUpInverse_(0),
        matrix_(1.f),
        inverse_(1.f) {}

    void set(const glm::mat4& matrix) {
        matrix_ = matrix;
        needUpInverse_ = 1;
    }

    const glm::mat4& get()const {
        return matrix_;
    }

    const glm::mat4& getInverse()const {
        if (needUpInverse_) {
            inverse_ = glm::inverse(matrix_);
            needUpInverse_ = 0;
        }
        return inverse_;
    }

private:
   
    mutable bool needUpInverse_;
    mutable glm::mat4 inverse_;
    glm::mat4 matrix_;
   
};

/// <summary>
/// ProjectionMatrix
/// </summary>
class ProjectionMatrix {

public:
    ProjectionMatrix() {}
    inline const MatrixShell& getMatrixShell()const {
        return matrix;
    }

    inline const ProjData& getData()const {
        return data;
    }

    inline const glm::mat4& get()const {
        return matrix.get();
    }

    inline const glm::mat4& getInverse()const {
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

private:

    MatrixShell matrix;
    ProjData data;

};

/// <summary>
/// BasisMatrix
/// </summary>
class BasisMatrix {

public:

    BasisMatrix() {
        needUpMatrix_ = 1;
    }

    inline const MatrixShell& getMatrixShell()const {
        upMatrix();
        return matrix_;
    }

    inline const glm::mat4& get()const {
        upMatrix();
        return matrix_.get();
    }

    inline const glm::mat4& getInverse()const {
        return matrix_.getInverse();
    }

    inline const Basis& getBasis()const {
        return basis_;
    }

    inline void setPosition(const glm::vec3& position) {
        if (basis_.position != position) {
            basis_.position = position;
            needUpMatrix_ = 1;
        }
    }

    inline void lookInDir(const glm::vec3& direction) {
        basis_.front = glm::normalize(-direction);
        needUpMatrix_ = 1;
    }

    void setBasis(const Basis& basis) {
        if (basis_ != basis) {
            basis_ = basis;
            needUpMatrix_ = 1;
        }
    }

private:

    mutable MatrixShell matrix_;
    mutable bool needUpMatrix_;

    Basis basis_;

    inline void upMatrix() const{
        if (needUpMatrix_) {
            matrix_.set(basis_.getMatrix());
            needUpMatrix_ = 0;
        }
    }
};

#endif

