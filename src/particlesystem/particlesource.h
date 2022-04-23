
#pragma once

// GLM headers.
#include <glm/glm.hpp>

struct ParticleSource
{
    ParticleSource() : position(0.0f), velocity(0.0f), particle_velocity(0.0f) {}
    ParticleSource(glm::vec3 position, glm::vec3 velocity, glm::vec3 particle_velocity) 
        : position(position)
        , velocity(velocity)
        , particle_velocity(particle_velocity) {}
        
    glm::vec3 position, velocity, particle_velocity;
};
