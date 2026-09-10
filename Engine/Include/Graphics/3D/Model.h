// Copyright (c) 2025 Adel Hales

#pragma once

#include <SFML/Graphics/Texture.hpp>

#include <string>
#include <vector>

#include "Mesh.h"

struct Material
{
    sf::Texture diffuse;
};

struct Model
{
    std::vector<Mesh> meshes;
    std::vector<Material> materials;
};

namespace ModelLoader
{
    bool Load(Model& model, const std::string& filename);
}