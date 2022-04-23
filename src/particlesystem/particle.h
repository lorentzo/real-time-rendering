
#pragma once

#include <glm/glm.hpp>

struct Particle
{
    // Default constructor.
    Particle() : position(0.0f), velocity(0.0f), color(1.0f), life(0.0f) {}

    // Parameters.
    glm::vec3 position, velocity;
    glm::vec4 color;
    float life;
};
