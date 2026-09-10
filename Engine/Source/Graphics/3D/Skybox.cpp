// Copyright (c) 2025 Adel Hales

#include "Graphics/3D/Skybox.h"

#include <SFML/Graphics/Image.hpp>

#include <glad/glad.h>

#include "Utils/Log.h"

static bool LoadFace(const std::string& filename, GLenum target)
{
    sf::Image image;
    if (!image.loadFromFile(filename))
    {
        LOG_ERROR("Failed to load cubemap face: {}", filename);
        return false;
    }

    const sf::Vector2u size = image.getSize();
    glTexImage2D(target, 0, GL_RGBA8, size.x, size.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, image.getPixelsPtr());

    return true;
}

bool SkyboxLoader::Load(Skybox& skybox, const std::string& folder)
{
    glGenVertexArrays(1, &skybox.vao);

    const std::pair<std::string, GLenum> faces[] =
    {
        { "right.png",  GL_TEXTURE_CUBE_MAP_POSITIVE_X },
        { "left.png",   GL_TEXTURE_CUBE_MAP_NEGATIVE_X },
        { "top.png",    GL_TEXTURE_CUBE_MAP_POSITIVE_Y },
        { "bottom.png", GL_TEXTURE_CUBE_MAP_NEGATIVE_Y },
        { "front.png",  GL_TEXTURE_CUBE_MAP_POSITIVE_Z },
        { "back.png",   GL_TEXTURE_CUBE_MAP_NEGATIVE_Z },
    };

    glGenTextures(1, &skybox.cubemap);
    glBindTexture(GL_TEXTURE_CUBE_MAP, skybox.cubemap);

    for (const auto& [face, target] : faces)
    {
        if (!LoadFace(folder + '/' + face, target))
        {
            return false;
        }
    }

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    return true;
}