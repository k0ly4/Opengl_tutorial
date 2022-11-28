#include "Camera.h"
/// <summary>
/// Camera
/// </summary>
//void Camera::move(const glm::vec3& vec) {
//    Basis basis = view.getBasis();
//    basis.position += vec.z * basis.front;
//    basis.position = basis.position + vec.x * glm::normalize(glm::cross(basis.front, GAME::WORLD_UP));
//    view.setBasis(basis);
//}

void Camera::mouse_move(glm::vec2 pos_mouse) {
    glm::vec2 offset(pos_mouse - cur_pos_mouse);
    cur_pos_mouse = pos_mouse;
    angle.yaw += offset.x * SENSITIVITY;
    angle.pitch += -offset.y * SENSITIVITY;
    angle.normalizePitch(89.f);

    Basis basis = view.getBasis();
    angle.getBasis(basis);
    basis.front = glm::normalize(basis.front);
    basis.right = glm::normalize(glm::cross(basis.front, GAME::WORLD_UP));
    basis.up = glm::normalize(glm::cross(basis.right, basis.front));
    view.setBasis(basis);
}
