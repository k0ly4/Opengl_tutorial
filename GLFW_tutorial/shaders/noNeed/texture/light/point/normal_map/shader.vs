#version 330 core

layout (location = 0) in vec3 in_pos;
layout (location = 1) in vec3 in_norm;
layout (location = 2) in vec2 in_tex_coord;
layout (location = 3) in vec3 in_tang;
layout (location = 4) in vec3 in_bitang;

uniform mat4 model;
uniform mat4 camera;
uniform mat4 projection;
uniform vec3 lightPos;
uniform vec3 viewPos;

out VS_OUT{
   vec2 texCoord;
   vec3 FragPos;
   vec3 lightDir;
   vec3 viewDir;
   vec3 lightPos;
   vec3 fragToLight;
}vs_out;

void main()
{
   vs_out.FragPos =vec3(model* vec4(in_pos, 1.f));
   vs_out.texCoord = in_tex_coord;
   gl_Position = projection* camera* vec4( vs_out.FragPos, 1.f);
   vec3  T =normalize(vec3(model*vec4(in_tang,0.f)));
   vec3  N =normalize(vec3(model*vec4(in_norm,0.f)));
   vec3  B =normalize(vec3(model*vec4(in_bitang,0.f)));
   
  mat3 TBN =transpose(mat3(T,B,N));
   vs_out.fragToLight = vs_out.FragPos-lightPos;
   vs_out.viewDir = TBN*normalize(viewPos-vs_out.FragPos);
   vs_out.lightDir = TBN*normalize (lightPos - vs_out.FragPos);
   vs_out.lightPos = TBN *lightPos;
   vs_out.FragPos = TBN* vs_out.FragPos;  
}