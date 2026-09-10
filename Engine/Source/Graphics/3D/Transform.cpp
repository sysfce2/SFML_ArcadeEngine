// Copyright (c) 2025 Adel Hales

#include "Graphics/3D/Transform.h"

glm::mat4 Transform::GetMatrix() const
{
    return glm::scale(glm::translate(glm::mat4(1), position) * glm::mat4(rotation), scale);
}

glm::vec3 Transform::GetRight() const
{
    return rotation * glm::vec3(1, 0, 0);
}

glm::vec3 Transform::GetUp() const
{
    return rotation * glm::vec3(0, 1, 0);
}

glm::vec3 Transform::GetForward() const
{
    return rotation * glm::vec3(0, 0, 1);
}