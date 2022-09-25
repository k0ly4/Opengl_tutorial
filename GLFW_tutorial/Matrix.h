
#ifndef MATRIX_H
#define MATRIX_H

#include "Transform.h"

/// <summary>
/// MatrixShell
/// </summary>
class MatrixShell {

public:
    MatrixShell() :needUpInverse(0), matrix(1.f) {}
    void set(const glm::mat4& matrix_) {
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

private:
    bool needUpInverse;
    glm::mat4 matrix;
    glm::mat4 inverse;
};

/// <summary>
/// cMatrix
/// </summary>
class cMatrix {

public:
    cMatrix(MatrixShell& shell_) :shell(shell_) {}
    const glm::mat4& get()const {
        return shell.get();
    }
    const glm::mat4& inverse()const {
        return shell.getInverse();
    }

private:
    MatrixShell& shell;
};

/// <summary>
/// ProjectionMatrix
/// </summary>
class ProjectionMatrix {

public:
    ProjectionMatrix() {}
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

private:

    MatrixShell matrix;
    bool needUpMatrix;
    Basis basis;

    inline void upMatrix() {
        if (needUpMatrix) {
            matrix.set(basis.lookAt());
        }
    }
};

#endif

