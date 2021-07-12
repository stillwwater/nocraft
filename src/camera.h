#ifndef CAMERA_H
#define CAMERA_H

#include <glad/glad.h>

#include "xmath.h"

struct Camera {
    Vector3 position   = Vector3(0);
    Vector2 rotation   = Vector2(-math::PIOver2, 0.0f);
    Vector3 front      = Vector3(0.0f, 0.0f, -1.0f);
    Vector3 world_up   = Vector3(0.0f, 1.0f, 0.0f);
    Vector3 up;
    Vector3 right;

    Camera() {
        update_rotation();
    }

    Camera(const Vector3 &position) : position{position} {
        update_rotation();
    }

    void update_rotation();
    Matrix4 view_matrix() const;
};

inline void Camera::update_rotation() {
    float pitch = rotation.y;
    float yaw   = rotation.x;

    front.x = cosf(yaw) * cosf(pitch);
    front.y = sinf(pitch);
    front.z = sinf(yaw) * cosf(pitch);
    math::normalize(&front);

    right = math::normalize(math::cross(front, world_up));
    up    = math::normalize(math::cross(right, front));
}

inline Matrix4 Camera::view_matrix() const {
    return math::lookat(position, position + front, up);
}

#endif // CAMERA_H
