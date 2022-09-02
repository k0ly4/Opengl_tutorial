#version 330 core

layout (location = 0) in vec3 in_pos;
layout (location =1) in vec3 a_norm;
layout (location = 2) in vec3 a_color;


uniform mat4 model;
uniform mat4 camera;
uniform mat4 projection;

out vec3 color;
out vec3 norm;
out vec3 FragPos;

void main()
{
   FragPos =vec3(model* vec4(in_pos, 1.f));

  norm =normalize(mat3(transpose(inverse(model))) *a_norm);
   color = a_color;
   gl_Position = projection* camera* vec4(FragPos, 1.f);
}