#version 330 core

layout (location = 0) in vec3 in_pos;
layout (location = 1) in vec2 in_texCoord;

out vec2 texCoord;

void main()
{  
    texCoord = in_texCoord;
    
   gl_Position = vec4(in_pos,1.f);
}