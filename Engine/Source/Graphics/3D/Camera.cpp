// Copyright (c) 2025 Adel Hales

#include "Graphics/3D/Camera.h"

glm::mat4 Camera::GetViewMatrix() const
{
    return glm::translate(glm::mat4(glm::conjugate(transform.rotation)), -transform.position);
}

glm::mat4 Camera::GetProjectionMatrix() const
{
    return glm::perspectiveLH(glm::radians(60.f), 1.f, 0.1f, 5000.f);
}