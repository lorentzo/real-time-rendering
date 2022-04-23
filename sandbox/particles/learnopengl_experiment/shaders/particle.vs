
#version 330 core

layout (location = 0) in vec3 in_position;
layout (location = 1) in vec3 in_normal;
layout (location = 2) in vec2 in_tex_coords;

out vec3 particle_world_position;
out vec3 particle_normal;
out vec3 particle_frag_position;
out vec2 tex_coords;
out vec4 particle_color;

// Particle.
uniform vec3 offset;
uniform vec4 color;
uniform float scale;
// Transform.
uniform vec4 projection;
uniform mat4 view;
uniform mat4 model;


void main()
{
    tex_coords = in_tex_coords;
    particle_color = color;

    vec4 curr_position = vec4(in_position * scale + offset, 1.0);

    particle_world_position = vec3(model * curr_position);
    particle_normal = mat3(model) * in_normal;   // scaling: Normal = mat3(transpose(inverse(model))) * aNormal;  
    particle_frag_position = vec3(model * curr_position); // TODO

    //gl_Position =  projection * vec4(in_position * scale + offset, 1.0);
    gl_Position =  projection * view * vec4(particle_world_position, 1.0);
}
