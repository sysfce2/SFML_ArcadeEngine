// Copyright (c) 2025 Adel Hales

#pragma once

#include "Config/SpaceFighterConfig.h"

namespace SpaceFighter
{
    enum Action
    {
        MoveForward, MoveBackward, MoveLeft, MoveRight,
        MoveUp, MoveDown, RollLeft, RollRight, Boost, Shoot
    };

    struct Player
    {
        Transform transform;
        glm::vec3 velocity;
        bool firstPerson;
        Cooldown shootCooldown;
        Cooldown shieldCooldown;
        sf::RectangleShape cockpit;
    };

    struct Cube
    {
        Transform transform;
        glm::vec3 direction;
        float speed;
        int level;
        sf::Color color;
    };

    struct Enemy
    {
        Transform transform;
        glm::vec3 center;
        glm::vec3 axis;
        float radius;
        sf::Angle angle;
        float speed;
    };

    struct Stats
    {
        int lives;
        int score;
        Cooldown finalCooldown;
        int highScore;
        sf::Text livesText{GetDefaultFont()};
        sf::Text scoreText{GetDefaultFont()};
        sf::Text finalCooldownText{GetDefaultFont()};
        sf::Text highScoreText{GetDefaultFont()};
    };

    struct Assets
    {
        Skybox* skybox;
        Model* spaceship;
        Model* cube;
    };
}