// Copyright (c) 2025 Adel Hales

#include "Managers/RenderManager3D.h"

#define GLAD_GL_IMPLEMENTATION
#include <glad/glad.h>

#include <SFML/Window/Context.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Utils/Verify.h"

RenderManager3D::RenderManager3D()
{
    VERIFY(gladLoadGL(sf::Context::getFunction));

    VERIFY(modelShader_.loadFromFile("Content/Shaders/Model.vert", "Content/Shaders/Model.frag"));
    VERIFY(skyboxShader_.loadFromFile("Content/Shaders/Skybox.vert", "Content/Shaders/Skybox.frag"));

    modelShader_.setUniform("uDiffuse", 0);
    skyboxShader_.setUniform("uSkybox", 0);

    const GLuint modelProgram = modelShader_.getNativeHandle();
    const GLuint skyboxProgram = skyboxShader_.getNativeHandle();

    glGenBuffers(1, &cameraBuffer_);
    glBindBuffer(GL_UNIFORM_BUFFER, cameraBuffer_);
    glBufferData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::mat4), nullptr, GL_DYNAMIC_DRAW);
    glBindBufferBase(GL_UNIFORM_BUFFER, 0, cameraBuffer_);
    glUniformBlockBinding(modelProgram, glGetUniformBlockIndex(modelProgram, "Camera"), 0);
    glUniformBlockBinding(skyboxProgram, glGetUniformBlockIndex(skyboxProgram, "Camera"), 0);

    glGenBuffers(1, &lightsBuffer_);
    glBindBuffer(GL_UNIFORM_BUFFER, lightsBuffer_);
    glBufferData(GL_UNIFORM_BUFFER, alignof(Light) + gMaxLights * sizeof(Light), nullptr, GL_DYNAMIC_DRAW);
    glBindBufferBase(GL_UNIFORM_BUFFER, 1, lightsBuffer_);
    glUniformBlockBinding(modelProgram, glGetUniformBlockIndex(modelProgram, "Lights"), 1);
}

void RenderManager3D::Begin3D(const Camera& camera, const Skybox& skybox, std::span<const Light> lights)
{
    SetCamera(camera);
    SetSkybox(skybox);
    SetLights(lights);

    glEnable(GL_DEPTH_TEST);
    glClear(GL_DEPTH_BUFFER_BIT);

    glUseProgram(modelShader_.getNativeHandle());
}

void RenderManager3D::SetCamera(const Camera& camera)
{
    const GLsizeiptr size = sizeof(glm::mat4);
    glBindBuffer(GL_UNIFORM_BUFFER, cameraBuffer_);
    glBufferSubData(GL_UNIFORM_BUFFER, 0, size, glm::value_ptr(camera.GetViewMatrix()));
    glBufferSubData(GL_UNIFORM_BUFFER, size, size, glm::value_ptr(camera.GetProjectionMatrix()));

    const glm::vec3 position = camera.transform.position;
    modelShader_.setUniform("uViewPosition", sf::Glsl::Vec3(position.x, position.y, position.z));
}

void RenderManager3D::SetSkybox(const Skybox& skybox)
{
    glUseProgram(skyboxShader_.getNativeHandle());
    glBindTexture(GL_TEXTURE_CUBE_MAP, skybox.cubemap);
    glBindVertexArray(skybox.vao);
    glDrawArrays(GL_TRIANGLES, 0, 3);
}

void RenderManager3D::SetLights(std::span<const Light> lights)
{
    const int count = std::min((int)lights.size(), gMaxLights);
    glBindBuffer(GL_UNIFORM_BUFFER, lightsBuffer_);
    glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(int), &count);
    glBufferSubData(GL_UNIFORM_BUFFER, alignof(Light), count * sizeof(Light), lights.data());
}

void RenderManager3D::Draw(const Model& model, const Transform& transform, sf::Color color)
{
    modelShader_.setUniform("uModel", sf::Glsl::Mat4(glm::value_ptr(transform.GetMatrix())));
    modelShader_.setUniform("uColor", sf::Glsl::Vec3(color.r, color.g, color.b) / 255.f);

    for (const Mesh& mesh : model.meshes)
    {
        if (mesh.materialIndex)
        {
            const Material& material = model.materials[*mesh.materialIndex];
            glBindTexture(GL_TEXTURE_2D, material.diffuse.getNativeHandle());
        }

        glBindVertexArray(mesh.vao);
        glDrawArrays(GL_TRIANGLES, 0, mesh.vertexCount);
    }
}

void RenderManager3D::End3D()
{
    glBindVertexArray(0);
    glUseProgram(0);

    glDisable(GL_DEPTH_TEST);
}