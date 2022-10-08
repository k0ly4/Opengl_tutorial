#version 330 core

layout (location = 0) in vec3 in_pos;
layout (location = 1) in vec3 a_normal;

uniform mat4 model;
uniform mat4 camera;
uniform mat4 projection;

out vec3 pos;
out vec3 norm;

void main()
{  
 
   norm = normalize(mat3(transpose(inverse(model)))*a_normal);
   pos = vec3(model* vec4(in_pos,1.f));
   gl_Position = projection* camera* vec4(pos,1.f);
}