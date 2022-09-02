#version 330 core

layout (location = 0) in vec3 in_pos;
layout (location =1) in vec3 a_norm;
layout (location = 2) in vec2 a_tex_coord;


uniform mat4 model;
uniform mat4 camera;
uniform mat4 projection;

out vec2 tex_coord;
out vec3 norm;
out vec3 FragPos;

void main()
{
  FragPos =vec3(model* vec4(in_pos, 1.f));
   norm =normalize(mat3(transpose(inverse(model))) *a_norm);
   tex_coord = a_tex_coord;
   gl_Position = projection* camera* vec4(FragPos, 1.f);
}