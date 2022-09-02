#version 330 core

layout (location = 0) in vec3 in_pos;

   uniform mat4 projection;
    uniform mat4 camera;

void main()
{ 
   gl_Position =projection* camera* vec4(in_pos,1.f);
}