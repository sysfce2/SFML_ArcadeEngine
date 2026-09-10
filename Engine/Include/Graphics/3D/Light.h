// Copyright (c) 2025 Adel Hales

#pragma once

#include <glm/vec3.hpp>

enum class LightType
{
    Directional, Point, Spot
};

struct alignas(16) Light
{
    glm::vec3 color{1};  LightType type;
    glm::vec3 position;  float intensity{1};
    glm::vec3 direction; float range{20};
};

inline const int gMaxLights = 8;