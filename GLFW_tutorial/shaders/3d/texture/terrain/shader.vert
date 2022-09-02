#version 330 core

layout (location = 0) in vec3 in_pos;
layout (location = 1) in vec3 in_color;
layout (location = 2) in vec3 in_norm;

    uniform  mat4 projection;
    uniform mat4 camera;
    uniform mat4 model;
    
out vec3 color;
out vec3 normal;

void main()
{  normal = in_norm;
    color = in_color;
   gl_Position = projection* camera* model*vec4(in_pos,1.f);
}