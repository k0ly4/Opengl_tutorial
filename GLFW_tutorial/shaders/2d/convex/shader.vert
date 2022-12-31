#version 330 core

layout (location = 0) in vec2 in_pos;
layout (location = 1) in vec4 in_color;

//uniform mat4 view;
uniform mat4 projection;
uniform mat4 model;

out vec4 color;

void main()
{  
   gl_Position =projection* model* vec4(in_pos,0.f,1.f);
   color = in_color;
}