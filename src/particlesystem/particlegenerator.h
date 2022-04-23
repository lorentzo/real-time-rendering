
#pragma once

// Renderer headers.
#include "particle.h"
#include "particlesource.h"
#include "./../shader.h"
#include "./../texture.h"
#include "./../camera.h"

// OpenGL headers.
#include <glad/glad.h>
#include <GLFW/glfw3.h>

// GLM headers.
#include <glm/glm.hpp>

// STL headers.
#include <vector>
#include <stdio.h>      /* printf, scanf, puts, NULL */
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */

class ParticleGenerator
{
    public:
    
    // Default Constructor.
    ParticleGenerator()
        : n_particles(100)
        , particle_source(ParticleSource())
    {
        initialize_particles();
        initialize_particle_geometry();
        //initialize_particle_texture();
        initialize_particle_shader();
        srand (time(NULL));
    }
    // Constructor
    ParticleGenerator(unsigned int n_particles, ParticleSource particle_source)
        : n_particles(n_particles)
        , particle_source(particle_source)
    {
        initialize_particles();
        initialize_particle_geometry();
        //initialize_particle_texture();
        initialize_particle_shader();
        srand (time(NULL));
    }

    void initialize_particles()
    {
        for (unsigned int i = 0; i < n_particles; ++i)
        {
            particles.push_back(Particle());
        }
    }

    void initialize_particle_geometry()
    {
        // set up mesh and attribute properties
        unsigned int VBO;
        float particle_quad[] = {
            0.0f, 1.0f, 0.0f, 1.0f,
            1.0f, 0.0f, 1.0f, 0.0f,
            0.0f, 0.0f, 0.0f, 0.0f,

            0.0f, 1.0f, 0.0f, 1.0f,
            1.0f, 1.0f, 1.0f, 1.0f,
            1.0f, 0.0f, 1.0f, 0.0f
        }; 
        glGenVertexArrays(1, &this->VAO);
        glGenBuffers(1, &VBO);
        glBindVertexArray(this->VAO);
        // fill mesh buffer
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(particle_quad), particle_quad, GL_STATIC_DRAW);
        // set mesh attributes
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
        glBindVertexArray(0);

        cout<< "PARTICLE::MESH_CONSTRUCTOR::DONE" << endl;
    }

    void initialize_particle_shader()
    {
        Shader shader("./../sandbox/particles/learnopengl_experiment/shaders/particle.vs", "./../sandbox/particles/learnopengl_experiment/shaders/particle.fs"); // NOTE: without geometry shader.
        this->shader = shader;
        cout<< "PARTICLE::SHADER_CONSTRUCTING::DONE" << endl;
    }

    /*
    void initialize_particle_texture()
    {
        stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis (before loading model)
        TextureLoader texture_loader;
        texture_loader.load("./../sandbox/particles/learnopengl_experiment/textures/tex.png", DIFFUSE, "albedoMap"); // TODO: name must be same as in fs
        this->texture = texture_loader.textures[0];
        cout<< "PARTICLE::TEXTURE_LOADING::DONE" << endl;
    }
    */

    void before_frame(float dt, unsigned int n_new_particles, glm::vec3 offset)
    {
        // Add new particles.
        for (unsigned int i = 0; i < n_new_particles; ++i)
        {
            int unused_particle = this->first_unused_particle();
            this->respawn_particle(this->particles[unused_particle], this->particle_source, offset);
        }
        // Update all particles.
        for (unsigned int i = 0; i < this->n_particles; ++i)
        {
            Particle &p = this->particles[i];
            p.life -= dt;
            if (p.life > 0.0f)
            {
                p.position -= p.velocity * dt;
                p.color.a -= dt * 2.5f;
            }
        }
    }

    unsigned int first_unused_particle()
    {
        // Try to find from last used to end.
        for (unsigned int i = this->last_used_particle; i < this->n_particles; ++i)
        {
            if (this->particles[i].life <= 0.0f)
            {
                this->last_used_particle = i;
                return i;
            }
        }
        // Try to find from start to last used.
        for (unsigned int i = 0; i < this->last_used_particle; ++i)
        {
            if (this->particles[i].life <= 0.0f)
            {
                last_used_particle = i;
                return i;
            }
        }
        // If all are alive, override first particle.
        this->last_used_particle = 0;
        return 0;
    }

    void respawn_particle(Particle &p, ParticleSource &ps, glm::vec3 offset)
    {
        float jitter_position = ((rand() % 100) - 50) / 10.0f;
        float random_color = 0.5f + ((rand() % 100) / 100.0f);
        p.position = ps.position + jitter_position + offset;
        p.color = glm::vec4(random_color, random_color, random_color, 1.0f);
        p.life = 1.0f;
        p.velocity = ps.particle_velocity;
    }

    void render(glm::mat4 view, glm::mat4 projection, glm::mat4 model)
    {
        // Config.
        glBlendFunc(GL_SRC_ALPHA, GL_ONE); // use additive blending to give it a 'glow' effect

        // Shader
        this->shader.use();

        // Draw particles.
        for (Particle p : particles)
        {
            if(p.life > 0.0f)
            {
                //cout << "POS:" << p.position.x << " " << p.position.y << " " << p.position.z << endl;
                //cout << "COL:" << p.color.x << " " << p.color.y << " " << p.color.z << endl;
                this->shader.setVec3("offset", p.position);
                this->shader.setVec4("color", p.color);
                this->shader.setFloat("scale", rand() * 10); // TODO: Random
                this->shader.setMat4("view", view);
                this->shader.setMat4("projection", projection);
                this->shader.setMat4("model", model);
                //glActiveTexture(GL_TEXTURE0); // active proper texture unit before binding
                //shader.setInt(texture.name, 0); // now set the sampler to the correct texture unit. NOTE: this name will be a texture sampler variable name in shader                
                //glBindTexture(GL_TEXTURE_2D, texture.id); // and finally bind the texture
                glBindVertexArray(this->VAO);
                glDrawArrays(GL_TRIANGLES, 0, 6);
                glBindVertexArray(0);
            }
        }

        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    }

    // Render data.
    unsigned int VAO;
    Shader shader;
    Texture texture;

    // Particle data.
    ParticleSource particle_source;
    unsigned int n_particles;
    std::vector<Particle> particles;
    unsigned int last_used_particle = 0;
};


