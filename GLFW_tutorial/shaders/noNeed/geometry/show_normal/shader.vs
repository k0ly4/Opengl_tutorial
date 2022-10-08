#version 330 core

layout (location = 0) in vec3 in_pos;
layout (location = 1) in vec3 in_norm;

out VS_OUT{
   vec3 normal;
}vs_out;
  
   uniform mat4 camera;
   uniform mat4 model;
void main()
{ 
   gl_Position =camera * model * vec4(in_pos,1.f);
   vs_out.normal = normalize(mat3(transpose(inverse(camera*model)))*in_norm);
}