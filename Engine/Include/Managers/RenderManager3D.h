// Copyright (c) 2025 Adel Hales

#pragma once

#include <SFML/Graphics/Shader.hpp>

#include "Graphics/3D/Camera.h"
#include "Graphics/3D/Light.h"
#include "Graphics/3D/Model.h"
#include "Graphics/3D/Skybox.h"

class RenderManager3D
{
private:
    sf::Shader modelShader_;
    sf::Shader skyboxShader_;

    unsigned cameraBuffer_;
    unsigned lightsBuffer_;

public:
    RenderManager3D();

    void Begin3D(const Camera& camera, const Skybox& skybox, std::span<const Light> lights);
    void Draw(const Model& model, const Transform& transform, sf::Color color = sf::Color::White);
    void End3D();

private:
    void SetCamera(const Camera& camera);
    void SetSkybox(const Skybox& skybox);
    void SetLights(std::span<const Light> lights);
};