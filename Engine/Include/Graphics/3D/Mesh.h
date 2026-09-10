// Copyright (c) 2025 Adel Hales

#pragma once

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

#include <optional>
#include <span>

struct Vertex
{
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 texCoords;
};

struct Mesh
{
    unsigned vao;
    unsigned vbo;
    int vertexCount;
    std::optional<int> materialIndex;

    Mesh(std::span<const Vertex> vertices);
};