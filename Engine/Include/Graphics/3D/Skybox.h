// Copyright (c) 2025 Adel Hales

#pragma once

#include <string>

struct Skybox
{
    unsigned vao;
    unsigned cubemap;
};

namespace SkyboxLoader
{
    bool Load(Skybox& skybox, const std::string& folder);
}