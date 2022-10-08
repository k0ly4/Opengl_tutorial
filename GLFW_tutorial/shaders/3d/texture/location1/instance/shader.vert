#version 330 core

layout (location = 0) in vec3 in_pos;
layout (location =1) in vec2 in_texCoord;
layout (location = 2) in mat4 model;

uniform  mat4 projection;
uniform mat4 camera;

out vec2 texCoord;

void main()
{  
    texCoord = in_texCoord;
   gl_Position = projection* camera*model*vec4(in_pos,1.f);
}