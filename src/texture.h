
#pragma once

#include <glad/glad.h> 
#include <stb_image.h>

#include <string>

using namespace std;

enum TextureTypes 
{
    DIFFUSE,
    SPECULAR,
    NORMAL,
    HEIGHT
};

struct Texture 
{
    unsigned int id;
    string name;
    TextureTypes type;
    string filepath;
};

