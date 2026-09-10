// Copyright (c) 2025 Adel Hales

#pragma once

#include "Graphics/3D/Transform.h"

struct Camera
{
    Transform transform;

    glm::mat4 GetViewMatrix() const;
    glm::mat4 GetProjectionMatrix() const;
};