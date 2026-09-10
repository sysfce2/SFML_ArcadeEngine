// Copyright (c) 2025 Adel Hales

#include "SpaceFighter.h"

using namespace SpaceFighter;

Game::Game(EngineContext& context) :
    Scene(context),
    laserSound(*ctx.resources.FetchSound(LASER_SOUND_FILENAME)),
    explosionSound(*ctx.resources.FetchSound(EXPLOSION_SOUND_FILENAME)),
    music(*ctx.resources.FetchMusic(SPACE_MUSIC_FILENAME))
{
    InitAssets();
    InitPlayer();
    InitStats();
    InitMusic();
}

void Game::InitAssets()
{
    assets.skybox = ctx.resources.FetchSkybox(SKYBOX_FOLDER);
    assets.spaceship = ctx.resources.FetchModel(SPACESHIP_MODEL_FILENAME);
    assets.cube = ctx.resources.FetchModel(CUBE_MODEL_FILENAME);
}

void Game::InitPlayer()
{
    player.shootCooldown.SetDuration(PLAYER_LASER_COOLDOWN_DURATION);
    player.shieldCooldown.SetDuration(PLAYER_SHIELD_COOLDOWN_DURATION);

    player.cockpit.setTexture(ctx.resources.FetchTexture(COCKPIT_TEXTURE_FILENAME));
    player.cockpit.setSize(gConfig.windowSize);
}

void Game::InitStats()
{
    stats.finalCooldown.SetDuration(STATS_FINAL_COOLDOWN_DURATION);

    stats.livesText.setFillColor(STATS_LIVES_TEXT_COLOR);
    stats.scoreText.setFillColor(STATS_SCORE_TEXT_COLOR);
    stats.finalCooldownText.setFillColor(STATS_COOLDOWN_TEXT_COLOR);
    stats.highScoreText.setFillColor(STATS_HIGH_SCORE_TEXT_COLOR);

    stats.livesText.setOutlineThickness(2);
    stats.scoreText.setOutlineThickness(2);
    stats.finalCooldownText.setOutlineThickness(2);
    stats.highScoreText.setOutlineThickness(2);

    stats.livesText.setPosition({15, 90});
    stats.scoreText.setPosition({15, 130});
    stats.finalCooldownText.setPosition({15, 170});
    stats.highScoreText.setPosition({15, 210});
}

void Game::InitMusic()
{
    music.setVolume(MUSIC_VOLUME);
    music.setLooping(true);
}

void Game::Start()
{
    ctx.cursor.SetVisible(false);
    ctx.cursor.SetGrabbed(true);

    BindInputs();

    StartPlayer();
    StartCamera();
    StartCubes();
    StartEnemies();
    StartLights();
    StartStats();
    StartMusic();
}

void Game::BindInputs()
{
    ctx.input.Bind(MoveForward,  Input::Keyboard{sf::Keyboard::Scan::W});
    ctx.input.Bind(MoveBackward, Input::Keyboard{sf::Keyboard::Scan::S});
    ctx.input.Bind(MoveLeft,     Input::Keyboard{sf::Keyboard::Scan::A});
    ctx.input.Bind(MoveRight,    Input::Keyboard{sf::Keyboard::Scan::D});
    ctx.input.Bind(MoveUp,       Input::Keyboard{sf::Keyboard::Scan::Space});
    ctx.input.Bind(MoveDown,     Input::Keyboard{sf::Keyboard::Scan::LControl});
    ctx.input.Bind(RollLeft,     Input::Keyboard{sf::Keyboard::Scan::Q});
    ctx.input.Bind(RollRight,    Input::Keyboard{sf::Keyboard::Scan::E});
    ctx.input.Bind(Boost,        Input::Keyboard{sf::Keyboard::Scan::LShift});
    ctx.input.Bind(Shoot,        Input::Mouse{sf::Mouse::Button::Left});

    ctx.input.Bind(MoveForward,  Input::Axis{sf::Joystick::Axis::Y, -0.5f, 0});
    ctx.input.Bind(MoveBackward, Input::Axis{sf::Joystick::Axis::Y,  0.5f, 0});
    ctx.input.Bind(MoveLeft,     Input::Axis{sf::Joystick::Axis::X, -0.5f, 0});
    ctx.input.Bind(MoveRight,    Input::Axis{sf::Joystick::Axis::X,  0.5f, 0});
    ctx.input.Bind(MoveUp,       Input::Gamepad{GamepadButton::South, 0});
    ctx.input.Bind(MoveDown,     Input::Gamepad{GamepadButton::East,  0});
    ctx.input.Bind(Boost,        Input::Gamepad{GamepadButton::L2, 0});
    ctx.input.Bind(Shoot,        Input::Gamepad{GamepadButton::R2, 0});
    ctx.input.Bind(RollLeft,     Input::Gamepad{GamepadButton::L1, 0});
    ctx.input.Bind(RollRight,    Input::Gamepad{GamepadButton::R1, 0});
}

void Game::StartPlayer()
{
    EventPlayerSpawn();

    player.firstPerson = true;
}

void Game::StartCamera()
{
    camera.transform = {};
}

void Game::StartCubes()
{
    cubes.clear();

    for (int i = 0; i < CUBE_COUNT; i++)
    {
        auto& cube = cubes.emplace_back();

        cube.transform.position = GetRandomPoint(CUBE_SPAWN_RADIUS);
        cube.transform.scale = {2, 2, 2};
        cube.direction = GetRandomDirection();
        cube.level = ctx.random.Int(0, 2);
        cube.speed = CUBE_SPEED * (cube.level + 1);
        cube.color = CUBE_COLORS[cube.level];
    }
}

void Game::StartEnemies()
{
    enemies.clear();

    for (int i = 0; i < ENEMY_COUNT; i++)
    {
        auto& enemy = enemies.emplace_back();

        enemy.center = GetRandomPoint(ENEMY_SPAWN_RADIUS);
        enemy.axis = GetRandomDirection();
        enemy.radius = ctx.random.Float(ENEMY_ORBIT_RADIUS_MIN, ENEMY_ORBIT_RADIUS_MAX);
        enemy.angle = ctx.random.Angle(sf::degrees(0), sf::degrees(360));
        enemy.speed = ctx.random.Float(ENEMY_ORBIT_SPEED_MIN, ENEMY_ORBIT_SPEED_MAX);
    }
}

void Game::StartLights()
{
    lights.clear();

    auto& sun = lights.emplace_back();
    sun.type = LightType::Directional;
    sun.direction = glm::normalize(glm::vec3(-0.3f, -1, -0.2f));
    sun.color = {1, 1, 1};
    sun.intensity = 0.9f;

    auto& spot = lights.emplace_back();
    spot.type = LightType::Spot;
    spot.color = {0.3f, 0.6f, 1};
    spot.intensity = 3;
    spot.range = 25;
}

void Game::StartStats()
{
    stats.lives = PLAYER_LIVES;
    stats.score = 0;
    stats.highScore = ctx.save.Get<int>(STATS_HIGH_SCORE_KEY);
    stats.finalCooldown.Restart();

    stats.livesText.setString("Lives : " + std::to_string(stats.lives));
    stats.scoreText.setString("Score : " + std::to_string(stats.score));
    stats.highScoreText.setString("Best : " + std::to_string(stats.highScore));
}

void Game::StartMusic()
{
    music.play();
}

void Game::OnEvent(const sf::Event& event)
{
    if (auto mouse = event.getIf<sf::Event::MouseMovedRaw>())
    {
        glm::vec3 eulers(mouse->delta.y, mouse->delta.x, 0);
        RotateLocal(player.transform, eulers * PLAYER_ROTATE_SPEED * ctx.time.GetDeltaTime());
    }

    if (auto keyboard = event.getIf<sf::Event::KeyPressed>())
    {
        if (keyboard->scancode == sf::Keyboard::Scan::C)
        {
            player.firstPerson = !player.firstPerson;
        }
    }

    if (auto joystick = event.getIf<sf::Event::JoystickButtonPressed>())
    {
        if (Input::HardwareToLogical(joystick->button, joystick->joystickId) == GamepadButton::L3)
        {
            player.firstPerson = !player.firstPerson;
        }
    }
}

void Game::Update()
{
    if (ctx.input.Pressed(Shoot) && player.shootCooldown.IsOver())
    {
        EventPlayerShoot();
        player.shootCooldown.Restart();
    }

    UpdatePlayer();
    UpdateCamera();
    UpdateCubes();
    UpdateEnemies();
    UpdateStats();

    HandleCollisions();

    if (stats.finalCooldown.IsOver())
    {
        ctx.save.Set(STATS_HIGH_SCORE_KEY, std::max(stats.score, stats.highScore));
        ctx.scenes.RestartCurrentScene();
    }
}

void Game::UpdatePlayer()
{
    UpdatePlayerRotation();
    UpdatePlayerRoll();
    UpdatePlayerMovement();
    UpdatePlayerSpotLight();
    UpdatePlayerCockpit();
}

void Game::UpdatePlayerRotation()
{
    sf::Vector2f direction(
        sf::Joystick::getAxisPosition(0, sf::Joystick::Axis::Z) / 100,
        sf::Joystick::getAxisPosition(0, sf::Joystick::Axis::R) / 100
    );

    if (direction.length() >= gConfig.joystickDeadzone)
    {
        glm::vec3 rotation(direction.y * std::abs(direction.y), direction.x * std::abs(direction.x), 0);
        RotateLocal(player.transform, rotation * PLAYER_ROTATE_JOYSTICK_SPEED * ctx.time.GetDeltaTime());
    }
}

void Game::UpdatePlayerRoll()
{
    float roll = 0;

    if (ctx.input.Pressed(RollLeft))  { roll++; }
    if (ctx.input.Pressed(RollRight)) { roll--; }

    float angle = glm::radians(roll * PLAYER_ROLL_SPEED * ctx.time.GetDeltaTime());
    glm::quat rotation = glm::angleAxis(angle, player.transform.GetForward());

    player.transform.rotation = glm::normalize(rotation * player.transform.rotation);
}

void Game::UpdatePlayerMovement()
{
    glm::vec3 direction(0);

    if (ctx.input.Pressed(MoveForward))  { direction += player.transform.GetForward(); }
    if (ctx.input.Pressed(MoveBackward)) { direction -= player.transform.GetForward(); }
    if (ctx.input.Pressed(MoveRight))    { direction += player.transform.GetRight(); }
    if (ctx.input.Pressed(MoveLeft))     { direction -= player.transform.GetRight(); }
    if (ctx.input.Pressed(MoveUp))       { direction += player.transform.GetUp(); }
    if (ctx.input.Pressed(MoveDown))     { direction -= player.transform.GetUp(); }

    if (direction != glm::vec3(0))
    {
        float thrust = ctx.input.Pressed(Boost) ? PLAYER_MOVE_SPEED_BOOST : PLAYER_MOVE_SPEED;
        player.velocity += glm::normalize(direction) * thrust * ctx.time.GetDeltaTime();
    }

    player.velocity *= std::pow(PLAYER_MOVE_DAMPING, ctx.time.GetDeltaTime());
    player.transform.position += player.velocity * ctx.time.GetDeltaTime();
}

void Game::UpdateCamera()
{
    glm::vec3 target = player.transform.position;

    if (!player.firstPerson)
    {
        float zOffset = CAMERA_TPS_Z_OFFSET_IDLE;

        if (ctx.input.Pressed(MoveForward) && !ctx.input.Pressed(MoveBackward))
        {
            zOffset = ctx.input.Pressed(Boost) ? CAMERA_TPS_Z_OFFSET_BOOST : CAMERA_TPS_Z_OFFSET_MOVE;
        }

        target += player.transform.GetUp() * CAMERA_TPS_Y_OFFSET;
        target -= player.transform.GetForward() * zOffset;
    }

    float speed = std::min(CAMERA_FOLLOW_SPEED * ctx.time.GetDeltaTime(), 1.f);

    camera.transform.position += (target - camera.transform.position) * speed;
    camera.transform.rotation = glm::slerp(camera.transform.rotation, player.transform.rotation, speed);
}

void Game::UpdatePlayerSpotLight()
{
    auto& spot = lights.back();

    spot.position = player.firstPerson ? camera.transform.position : player.transform.position;
    spot.direction = camera.transform.GetForward();
}

void Game::UpdatePlayerCockpit()
{
    if (!player.shieldCooldown.IsOver())
    {
        player.cockpit.setFillColor(PLAYER_COCKPIT_SHIELD_COLOR);
        return;
    }

    float t = glm::clamp(glm::length(player.transform.position) / PLAYER_MAP_LIMIT_RADIUS, 0.f, 1.f);

    sf::Color safe = PLAYER_COCKPIT_SAFE_COLOR;
    sf::Color danger = PLAYER_COCKPIT_DANGER_COLOR;

    player.cockpit.setFillColor({
        std::uint8_t(safe.r + (danger.r - safe.r) * t),
        std::uint8_t(safe.g + (danger.g - safe.g) * t),
        std::uint8_t(safe.b + (danger.b - safe.b) * t)
    });
}

void Game::UpdateCubes()
{
    for (auto& cube : cubes)
    {
        cube.transform.position += cube.direction * cube.speed * ctx.time.GetDeltaTime();

        glm::vec3 position = cube.transform.position;

        if (glm::dot(position, position) > std::pow(CUBE_SPAWN_RADIUS, 2))
        {
            cube.direction *= -1;
        }
    }
}

void Game::UpdateEnemies()
{
    for (auto& enemy : enemies)
    {
        enemy.angle += sf::degrees(enemy.speed) * ctx.time.GetDeltaTime();

        glm::vec3 radial = glm::normalize(glm::cross(enemy.axis, {0, 1, 0})) * enemy.radius;
        glm::vec3 orbit = glm::angleAxis(enemy.angle.asRadians(), enemy.axis) * radial;
        glm::vec3 forward = glm::normalize(glm::cross(orbit, enemy.axis));

        enemy.transform.position = enemy.center + orbit;
        enemy.transform.rotation = glm::quatLookAt(forward, {0, 1, 0});
    }
}

void Game::UpdateStats()
{
    float time = STATS_FINAL_COOLDOWN_DURATION - stats.finalCooldown.GetElapsedTime();
    stats.finalCooldownText.setString("Time : " + std::to_string((int)std::ceil(time)));
}

void Game::EventPlayerSpawn()
{
    player.transform = {};
    player.velocity = {};
    player.shootCooldown.Restart();
    player.shieldCooldown.Restart();
}

void Game::EventPlayerShoot()
{
    glm::vec3 direction = camera.transform.GetForward();

    for (auto& cube : cubes)
    {
        glm::vec3 toCube = cube.transform.position - camera.transform.position;
        float depth = glm::dot(toCube, direction);

        if (depth > 0)
        {
            glm::vec3 offset = toCube - direction * depth;

            if (glm::dot(offset, offset) <= std::pow(CUBE_HITBOX_RADIUS, 2))
            {
                EventCubeDead(cube);
                return;
            }
        }
    }
}

void Game::EventPlayerDead()
{
    explosionSound.play();

    stats.lives--;
    stats.livesText.setString("Lives : " + std::to_string(stats.lives));

    if (stats.lives == 0)
    {
        LOG_INFO("You Lose!");
        ctx.scenes.RestartCurrentScene();
    }
    else
    {
        EventPlayerSpawn();
    }
}

void Game::EventCubeDead(Cube& cube)
{
    laserSound.play();

    cube.transform.position = GetRandomPoint(CUBE_SPAWN_RADIUS);
    cube.direction = GetRandomDirection();

    stats.score++;
    stats.scoreText.setString("Score : " + std::to_string(stats.score));
}

void Game::HandleCollisions()
{
    if (player.shieldCooldown.IsOver())
    {
        HandleCollisionsPlayerMap();
        HandleCollisionsPlayerEnemies();
    }
}

void Game::HandleCollisionsPlayerMap()
{
    glm::vec3 position = player.transform.position;

    if (glm::dot(position, position) > std::pow(PLAYER_MAP_LIMIT_RADIUS, 2))
    {
        EventPlayerDead();
    }
}

void Game::HandleCollisionsPlayerEnemies()
{
    for (auto& enemy : enemies)
    {
        glm::vec3 direction = enemy.transform.position - player.transform.position;

        if (glm::dot(direction, direction) < std::pow(ENEMY_HITBOX_RADIUS, 2))
        {
            EventPlayerDead();
            return;
        }
    }
}

glm::vec3 Game::GetRandomDirection() const
{
    return glm::normalize(glm::vec3(
        ctx.random.Float(-1, 1),
        ctx.random.Float(-1, 1),
        ctx.random.Float(-1, 1))
    );
}

glm::vec3 Game::GetRandomPoint(float radius) const
{
    return GetRandomDirection() * std::cbrt(ctx.random.Float(0, 1)) * radius;
}

void Game::RotateLocal(Transform& transform, glm::vec3 eulers) const
{
    glm::vec3 angles = glm::radians(eulers);

    transform.rotation = glm::angleAxis(angles.x, transform.GetRight())   * transform.rotation;
    transform.rotation = glm::angleAxis(angles.y, transform.GetUp())      * transform.rotation;
    transform.rotation = glm::angleAxis(angles.z, transform.GetForward()) * transform.rotation;

    transform.rotation = glm::normalize(transform.rotation);
}

void Game::Render() const
{
    ctx.renderer3D.Begin3D(camera, *assets.skybox, lights);

    if (!player.firstPerson)
    {
        ctx.renderer3D.Draw(*assets.spaceship, player.transform);
    }

    for (const auto& cube : cubes)
    {
        ctx.renderer3D.Draw(*assets.cube, cube.transform, cube.color);
    }

    for (const auto& enemy : enemies)
    {
        ctx.renderer3D.Draw(*assets.spaceship, enemy.transform);
    }

    ctx.renderer3D.End3D();

    ctx.renderer.Draw(player.cockpit);
    ctx.renderer.Draw(stats.livesText);
    ctx.renderer.Draw(stats.scoreText);
    ctx.renderer.Draw(stats.finalCooldownText);
    ctx.renderer.Draw(stats.highScoreText);
}

void Game::OnPause(bool paused)
{
    if (paused)
    {
        player.shootCooldown.Stop();
        stats.finalCooldown.Stop();
        music.pause();
    }
    else
    {
        player.shootCooldown.Start();
        stats.finalCooldown.Start();
        music.play();
    }
}

void Game::OnCleanup()
{
    music.stop();
    explosionSound.stop();
}