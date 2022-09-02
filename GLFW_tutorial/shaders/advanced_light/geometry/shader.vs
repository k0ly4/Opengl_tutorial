#version 330 core

layout (location = 0) in vec3 in_pos;
layout (location = 1) in vec3 a_norm;
layout (location = 2) in vec2 a_tex_coord;


uniform mat4 model;
uniform mat4 camera;
uniform mat4 projection;
uniform bool inverte_normal;

out VS_OUT{
   vec2 texCoord;
   vec3  norm;
   vec3 FragPos;
}vs_out;

void main()
{
   vec4 viewPos  = camera*model*vec4(in_pos,1.f);

   vs_out.FragPos = viewPos.xyz;
   vs_out.texCoord = a_tex_coord;

  vs_out.norm = transpose(inverse(mat3(camera*model)))*a_norm;
   if(inverte_normal)
   vs_out.norm = -vs_out.norm;
   
   gl_Position = projection* viewPos;
  
}