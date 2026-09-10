// Copyright (c) 2025 Adel Hales

#include "Graphics/3D/Model.h"

#define TINYOBJLOADER_IMPLEMENTATION
#include <tinyobjloader/tiny_obj_loader.h>

#include "Utils/Log.h"

static Vertex BuildVertex(const tinyobj::attrib_t& attrib, const tinyobj::index_t& index)
{
    Vertex vertex;

    const auto position = &attrib.vertices[index.vertex_index * 3];
    vertex.position = {position[0], position[1], position[2]};

    if (index.normal_index >= 0)
    {
        const auto normal = &attrib.normals[index.normal_index * 3];
        vertex.normal = {normal[0], normal[1], normal[2]};
    }

    if (index.texcoord_index >= 0)
    {
        const auto texCoords = &attrib.texcoords[index.texcoord_index * 2];
        vertex.texCoords = {texCoords[0], 1 - texCoords[1]};
    }

    return vertex;
}

static Mesh BuildMesh(const tinyobj::attrib_t& attrib, const tinyobj::shape_t& shape)
{
    std::vector<Vertex> vertices;

    vertices.reserve(shape.mesh.indices.size());

    for (const auto& index : shape.mesh.indices)
    {
        vertices.push_back(BuildVertex(attrib, index));
    }

    Mesh mesh(vertices);

    if (!shape.mesh.material_ids.empty())
    {
        mesh.materialIndex = shape.mesh.material_ids[0];
    }

    return mesh;
}

static sf::Texture BuildTexture(const std::string& filename)
{
    sf::Texture texture;

    if (!texture.loadFromFile(filename))
    {
        LOG_ERROR("Failed to load texture: {}", filename);
        return {};
    }

    texture.setSmooth(true);

    if (!texture.generateMipmap())
    {
        LOG_WARNING("Failed to generate mipmaps for texture: {}", filename);
    }

    return texture;
}

static Material BuildMaterial(const tinyobj::material_t& data, const std::string& directory)
{
    Material material;

    if (!data.diffuse_texname.empty())
    {
        material.diffuse = BuildTexture(directory + '/' + data.diffuse_texname);
    }

    return material;
}

bool ModelLoader::Load(Model& model, const std::string& filename)
{
    tinyobj::ObjReader reader;

    if (!reader.ParseFromFile(filename))
    {
        LOG_ERROR("Failed to load OBJ: {} ({})", filename, reader.Error());
        return false;
    }

    if (!reader.Warning().empty())
    {
        LOG_WARNING("OBJ warning: {} ({})", filename, reader.Warning());
    }

    const std::string directory = std::filesystem::path(filename).parent_path().string();

    for (const auto& data : reader.GetMaterials())
    {
        model.materials.push_back(BuildMaterial(data, directory));
    }

    for (const auto& shape : reader.GetShapes())
    {
        model.meshes.push_back(BuildMesh(reader.GetAttrib(), shape));
    }

    return true;
}