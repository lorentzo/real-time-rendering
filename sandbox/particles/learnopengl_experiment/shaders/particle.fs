
#version 330 core

in vec3 particle_world_position;
in vec3 particle_normal;
in vec3 particle_frag_position;
in vec2 tex_coords;
in vec4 particle_color;

out vec4 frag_color;

uniform sampler2D sprite;

void main()
{
    frag_color = particle_color;
    //frag_color = (texture(sprite, tex_coords) * particle_color);
}
