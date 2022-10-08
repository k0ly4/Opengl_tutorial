#version 330 core

layout (location = 0) in vec3 in_pos;
layout (location = 1) in vec4 in_color;

uniform  mat4 projection;
uniform mat4 camera;
uniform mat4 model;

out vec4 baseColor;

void main()
{  
   gl_Position = projection* camera* model*vec4(in_pos,1.f);
   baseColor = in_color;
}