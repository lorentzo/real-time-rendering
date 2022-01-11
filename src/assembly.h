
#pragma once

#include "mesh.h"
#include "texture.h"
#include "light.h"
#include "shader.h"

#include <vector>
#include <string>

//
// Assembly contains elements which describe an object. It contains:
// 1. meshes
// 2. shaders
// 3. textures
// 2. materials (shaders + textures)
//
// Based on those elements, objects are constructed:
// 1. position in the world
// 2. mesh-material relation
//
// Scene is constructed using assemblies.
//

class Assembly 
{
  public:
    Assembly(std::vector<Mesh> meshes, std::vector<Texture> textures, Shader shader, std::vector<PointLight> point_lights)
    {
        this->meshes = meshes;
        this->textures = textures;
        this->shader = shader;
        this->point_lights = point_lights;
    }

    std::vector<Mesh> meshes;
    std::vector<Texture> textures;
    Shader shader;
    std::vector<PointLight> point_lights;
};