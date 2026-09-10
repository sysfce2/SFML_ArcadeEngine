// Copyright (c) 2025 Adel Hales

#pragma once

#include "Scene/Scene.h"

namespace SpaceFighter
{
    const int PLAYER_LIVES = 3;
    const float PLAYER_MOVE_SPEED = 50;
    const float PLAYER_MOVE_SPEED_BOOST = 100;
    const float PLAYER_MOVE_DAMPING = 0.25f;
    const float PLAYER_ROTATE_SPEED = 30;
    const float PLAYER_ROTATE_JOYSTICK_SPEED = 120;
    const float PLAYER_ROLL_SPEED = 60;
    const float PLAYER_LASER_COOLDOWN_DURATION = 0.2f;
    const float PLAYER_SHIELD_COOLDOWN_DURATION = 3;
    const float PLAYER_MAP_LIMIT_RADIUS = 200;

    const float CAMERA_FOLLOW_SPEED = 5;
    const float CAMERA_TPS_Y_OFFSET = 10;
    const float CAMERA_TPS_Z_OFFSET_IDLE = 35;
    const float CAMERA_TPS_Z_OFFSET_MOVE = 38;
    const float CAMERA_TPS_Z_OFFSET_BOOST = 45;

    const int CUBE_COUNT = 50;
    const float CUBE_HITBOX_RADIUS = 5;
    const float CUBE_SPEED = 2.5f;
    const float CUBE_SPAWN_RADIUS = 150;

    const int ENEMY_COUNT = 20;
    const float ENEMY_HITBOX_RADIUS = 10;
    const float ENEMY_ORBIT_RADIUS_MIN = 50;
    const float ENEMY_ORBIT_RADIUS_MAX = 100;
    const float ENEMY_ORBIT_SPEED_MIN = 20;
    const float ENEMY_ORBIT_SPEED_MAX = 40;
    const float ENEMY_SPAWN_RADIUS = 120;

    const float STATS_FINAL_COOLDOWN_DURATION = 120;

    const float MUSIC_VOLUME = 10;

    const sf::Color PLAYER_COCKPIT_SHIELD_COLOR(70, 130, 190);
    const sf::Color PLAYER_COCKPIT_SAFE_COLOR(100, 180, 140);
    const sf::Color PLAYER_COCKPIT_DANGER_COLOR(180, 80, 60);
    const sf::Color STATS_LIVES_TEXT_COLOR(200, 110, 90);
    const sf::Color STATS_SCORE_TEXT_COLOR(90, 160, 210);
    const sf::Color STATS_COOLDOWN_TEXT_COLOR(120, 200, 140);
    const sf::Color STATS_HIGH_SCORE_TEXT_COLOR(210, 190, 120);
    const std::vector<sf::Color> CUBE_COLORS = {{90, 170, 220}, {170, 100, 220}, {220, 90, 90}};

    const std::string SKYBOX_FOLDER = "Galaxy";
    const std::string CUBE_MODEL_FILENAME = "Cube/Cube.obj";
    const std::string SPACESHIP_MODEL_FILENAME = "Spaceship/Spaceship.obj";
    const std::string COCKPIT_TEXTURE_FILENAME = "Cockpit.png";
    const std::string LASER_SOUND_FILENAME = "Laser.mp3";
    const std::string EXPLOSION_SOUND_FILENAME = "Explosion.mp3";
    const std::string SPACE_MUSIC_FILENAME = "Space.mp3";

    const std::string_view STATS_HIGH_SCORE_KEY = "Space Fighter:High Score";
}