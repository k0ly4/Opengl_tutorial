#version 330 core

layout (location = 0) in vec3 in_pos;
layout (location = 2) in vec2 in_tex_coord;

uniform mat4 model;
uniform mat4 camera;
uniform mat4 projection;

out vec2 TexCoord;
out vec3 FragPos;

void main()
{
  FragPos =vec3(model* vec4(in_pos, 1.f));
  TexCoord = in_tex_coord;
  gl_Position = projection* camera* vec4(FragPos, 1.f)
}