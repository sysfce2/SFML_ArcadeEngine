// Copyright (c) 2025 Adel Hales

#pragma once

#include <glm/gtc/quaternion.hpp>

struct Transform
{
    glm::vec3 position = {0, 0, 0};
    glm::quat rotation = {1, 0, 0, 0};
    glm::vec3 scale    = {1, 1, 1};

    glm::mat4 GetMatrix() const;
    glm::vec3 GetRight() const;
    glm::vec3 GetUp() const;
    glm::vec3 GetForward() const;
};