
#pragma once

#include "texture.h"

#include <string>
#include <vector>

using namespace std;

class TextureLoader
{
  public:
    TextureLoader() {}

    void load(string filepath, TextureTypes texture_type, string name)
    {
        Texture texture;
        texture.id = texture_from_file(filepath);
        texture.name = name;
        texture.type = texture_type;
        texture.filepath = filepath;
        textures.push_back(texture);
    }

    vector<Texture> textures;
  
  private:
    // TODO: gamma correction?
    unsigned int texture_from_file(string filepath)
    {
        unsigned int textureID;
        glGenTextures(1, &textureID);

        int width, height, nrComponents;
        unsigned char *data = stbi_load(filepath.c_str(), &width, &height, &nrComponents, 0);
        if (data)
        {
            GLenum format;
            if (nrComponents == 1)
                format = GL_RED;
            else if (nrComponents == 3)
                format = GL_RGB;
            else if (nrComponents == 4)
                format = GL_RGBA;

            glBindTexture(GL_TEXTURE_2D, textureID);
            glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

            stbi_image_free(data);
        }
        else
        {
            std::cout << "Texture failed to load at path: " << filepath << std::endl;
            stbi_image_free(data);
        }

        return textureID;
    }
};
