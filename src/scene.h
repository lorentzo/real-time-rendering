
#pragma once

#include "assembly.h"
#include "camera.h"
#include "meshloader.h"
#include "textureloader.h"
#include "light.h"
#include "shader.h"
#include "frame.h"
#include "particlesystem/particle.h"
#include "particlesystem/particlegenerator.h"
#include "particlesystem/particlesource.h"

#include <glm/glm.hpp>

#include <vector>
#include <string>

using namespace std;

//
// Scene contains one or more assemblies.
//


class Scene
{
  public:
    Scene(int frame_width, int frame_height)
    {
        // Mesh
        MeshLoader mesh_loader("./../sandbox/data/geometry/ground.obj");
        cout<< "SCENE::CONSTRUCTOR::MESH_LOADING::DONE" << endl;

        // Texture
        stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis (before loading model)
        TextureLoader texture_loader;
        texture_loader.load("./../sandbox/data/texture/texture.png", DIFFUSE, "albedoMap"); // TODO: name must be same as in fs
        cout<< "SCENE::CONSTRUCTOR::TEXTURE_LOADING::DONE" << endl;

        // Shader
        Shader shader("./../sandbox/data/shader/vertex.glsl", "./../sandbox/data/shader/fragment.glsl"); // NOTE: without geometry shader.
        cout<< "SCENE::CONSTRUCTOR::SHADER_CONSTRUCTING::DONE" << endl;

        // Light
        std::vector<PointLight> point_lights;
        PointLight point_light(glm::vec3(3.0, 3.0, 10.0), glm::vec3(1.0, 1.0, 1.0));
        point_lights.push_back(point_light);
        cout<< "SCENE::CONSTRUCTOR::POINT_LIGHT_CONSTRUCTING::DONE" << endl;

        // Assembly
        Assembly assembly(mesh_loader.meshes, texture_loader.textures, shader, point_lights);
        assemblies.push_back(assembly);
        cout<< "SCENE::CONSTRUCTOR::ASSEMBLY_CONSTRUCTING::DONE" << endl;

        // Prepare cameras.
        Camera camera("camera");
        active_camera = "camera";
        cameras.push_back(camera);
        cout<< "SCENE::CONSTRUCTOR::CAMERA_CONSTRUCTING::DONE" << endl;

        // Prepare frame info.
        frame = Frame(frame_width, frame_height);
        cout<< "SCENE::CONSTRUCTOR::FRAME_CONSTRUCTING::DONE" << endl;

        // Particle system.
        ParticleSource particle_source(glm::vec3(4.0f), glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        this->particle_system = ParticleGenerator(100, particle_source);

        // Scene meta.
        scene_name = "Scene 1";
        cout<< "SCENE::CONSTRUCTOR::DONE" << endl;
    }

    void before_render()
    {
        // configure global opengl state
        // -----------------------------
        glEnable(GL_DEPTH_TEST);
    }

    void before_frame(float dt)
    {
        Camera camera = get_active_camera();
        glm::mat4 view = camera.GetViewMatrix();
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)frame.width / (float)frame.height, 0.1f, 100.0f);
        glm::mat4 model = glm::mat4(1.0f);
        //model = glm::translate(model, glm::vec3(0.0, 0.0, 0.0); // TODO: TRS matrix
        //model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
        for (int assembly_i = 0; assembly_i < assemblies.size(); ++assembly_i)
        {
            // Fetch current assembly.
            Assembly assembly = assemblies[assembly_i];
            // Fetch assembly shader.
            Shader shader = assembly.shader;
            shader.use();
            // Set shader uniforms.
            shader.setMat4("view", view);
            shader.setMat4("projection", projection); // for camera
            shader.setMat4("model", model);
            shader.setVec3("camPos", camera.Position);
            // Bind textures.
            for (int texture_i = 0; texture_i < assembly.textures.size(); ++texture_i)
            {
                Texture texture = assembly.textures[texture_i];
                glActiveTexture(GL_TEXTURE0 + texture_i); // active proper texture unit before binding
                shader.setInt(texture.name, texture_i); // now set the sampler to the correct texture unit. NOTE: this name will be a texture sampler variable name in shader                
                glBindTexture(GL_TEXTURE_2D, texture.id); // and finally bind the texture
            }
            // Set light positions and colors. NOTE: only point lights for now.
            for (int light_i = 0; light_i < assembly.point_lights.size(); ++light_i) // TODO: max 4 in fs
            {
                shader.setVec3("lightPositions[" + std::to_string(light_i) + "]", assembly.point_lights[light_i].position);
                shader.setVec3("lightColors[" + std::to_string(light_i) + "]", assembly.point_lights[light_i].color);
            }
            // Bind vertices (VAO) for all meshes in assembly. NOTE: all meshes in assembly use same shaders (textures and materials).
            for (int mesh_i = 0; mesh_i < assembly.meshes.size(); ++mesh_i)
            {
                // TODO: add this as function to mesh so that drawing options can be changed!
                Mesh mesh = assembly.meshes[mesh_i];
                glBindVertexArray(mesh.VAO);
                glDrawElements(GL_TRIANGLE_STRIP, mesh.indices.size(), GL_UNSIGNED_INT, 0);
                glBindVertexArray(0); // set back to default
                glActiveTexture(GL_TEXTURE0); // set back to default
            }
        }
        // Particles.
        this->particle_system.before_frame(dt, 2, glm::vec3(2.0f, 3.0f, 3.0f));
        this->particle_system.render(view, projection, model);
    }

    ParticleGenerator particle_system;
    vector<Assembly> assemblies;
    vector<Camera> cameras; 
    std::string active_camera;
    Frame frame;
    string scene_name;

    Camera get_active_camera()
    {
        for (int camera_i = 0; camera_i < cameras.size(); ++camera_i)
        {
            if (cameras[camera_i].name == active_camera)
            {
                return cameras[camera_i];
            }
        }
    }

    Camera* get_active_camera_pointer()
    {
        for (int camera_i = 0; camera_i < cameras.size(); ++camera_i)
        {
            if (cameras[camera_i].name == active_camera)
            {
                return &cameras[camera_i];
            }
        }
        return NULL;
    }
};
