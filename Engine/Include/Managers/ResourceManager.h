// Copyright (c) 2025 Adel Hales

#pragma once

#include <SFML/Audio/Music.hpp>
#include <SFML/Audio/SoundBuffer.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Texture.hpp>

#include <optional>
#include <string>
#include <unordered_map>

#include "Graphics/3D/Model.h"
#include "Graphics/3D/Skybox.h"

class ResourceManager
{
private:
    std::unordered_map<std::string, sf::Texture> textures_;
    std::unordered_map<std::string, sf::SoundBuffer> sounds_;
    std::unordered_map<std::string, sf::Font> fonts_;
    std::unordered_map<std::string, Model> models_;
    std::unordered_map<std::string, Skybox> skyboxes_;

public:
    sf::Texture* FetchTexture(const std::string& filename);
    sf::SoundBuffer* FetchSound(const std::string& filename);
    sf::Font* FetchFont(const std::string& filename);
    Model* FetchModel(const std::string& filename);
    Skybox* FetchSkybox(const std::string& folder);
    std::optional<sf::Music> FetchMusic(const std::string& filename) const;
};