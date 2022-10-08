#version 330 core

layout (location = 0) in vec3 in_pos;
layout (location = 1) in vec2 in_tex_coord;

uniform  mat4 projection;

out vec2 texCoord;

void main()
{  
    texCoord = in_tex_coord;
   gl_Position = projection* vec4(in_pos,1.f);
}