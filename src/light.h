
#pragma once

#include <glm/glm.hpp>

using namespace std;

class PointLight
{ 
  public:
    PointLight(glm::vec3 position, glm::vec3 color)
    {
        this->position = position;
        this->color = color;
    }
    glm::vec3 position;
    glm::vec3 color;
};
