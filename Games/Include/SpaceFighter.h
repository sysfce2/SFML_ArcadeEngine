// Copyright (c) 2025 Adel Hales

#pragma once

#include "Types/SpaceFighterTypes.h"

namespace SpaceFighter
{
    class Game : public Scene
    {
    private:
        Player player;
        Camera camera;
        std::vector<Cube> cubes;
        std::vector<Enemy> enemies;
        std::vector<Light> lights;
        Stats stats;
        Assets assets;
        sf::Sound laserSound;
        sf::Sound explosionSound;
        sf::Music music;

    public:
        Game(EngineContext&);

        void Start();
        void OnEvent(const sf::Event&);
        void Update();
        void Render() const;
        void OnPause(bool);
        void OnCleanup();

    private:
        void InitAssets();
        void InitPlayer();
        void InitStats();
        void InitMusic();

        void BindInputs();

        void StartPlayer();
        void StartCamera();
        void StartCubes();
        void StartEnemies();
        void StartLights();
        void StartStats();
        void StartMusic();

        void UpdatePlayer();
        void UpdatePlayerRotation();
        void UpdatePlayerRoll();
        void UpdatePlayerMovement();
        void UpdatePlayerSpotLight();
        void UpdatePlayerCockpit();
        void UpdateCamera();
        void UpdateCubes();
        void UpdateEnemies();
        void UpdateStats();

        void EventPlayerSpawn();
        void EventPlayerShoot();
        void EventPlayerDead();
        void EventCubeDead(Cube& cube);

        void HandleCollisions();
        void HandleCollisionsPlayerMap();
        void HandleCollisionsPlayerEnemies();

        glm::vec3 GetRandomDirection() const;
        glm::vec3 GetRandomPoint(float radius) const;
        void RotateLocal(Transform& transform, glm::vec3 eulers) const;
    };
}