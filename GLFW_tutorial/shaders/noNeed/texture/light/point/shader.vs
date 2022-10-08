#version 330 core

layout (location = 0) in vec3 in_pos;
layout (location = 1) in vec3 a_norm;
layout (location = 2) in vec2 a_tex_coord;


uniform mat4 model;
uniform mat4 camera;
uniform mat4 projection;

out VS_OUT{
   vec2 texCoord;
   vec3  norm;
   vec3 FragPos;
}vs_out;

void main()
{
   vs_out.FragPos =vec3(model* vec4(in_pos, 1.f));
   vs_out.norm =normalize(mat3( transpose(inverse(model))) *a_norm);
   vs_out.texCoord = a_tex_coord;
   gl_Position = projection* camera* vec4( vs_out.FragPos, 1.f);
}