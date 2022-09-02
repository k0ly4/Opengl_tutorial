#version 330 core

layout (location = 0) in vec3 in_pos;
layout (location = 1) in vec3 in_norm;
layout (location = 2) in vec2 in_texCoord;
layout (location = 3) in mat4 instanceMatrix;

uniform  mat4 projection;
uniform  mat4 camera;


out vec2 texCoord;

void main()
{  
    texCoord = in_texCoord;
   gl_Position = projection* camera* instanceMatrix*vec4(in_pos,1.f);
}