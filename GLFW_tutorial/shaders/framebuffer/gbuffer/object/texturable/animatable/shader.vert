#version 330 core

layout (location = 0) in vec3 in_pos;
layout (location =1) in vec3 a_norm;
layout (location = 2) in vec2 a_tex_coord;

layout (location =3) in ivec4 a_id_bone1;
layout (location =4) in ivec4 a_id_bone2;
layout (location = 5) in vec4 a_weight1;
layout (location = 6) in vec4 a_weight2;

uniform mat4 model;
uniform mat4 camera;
uniform mat4 projection;

out vec2 tex_coord;
out vec3 norm;
out vec3 FragPos;
flat out float boneId[8];
out float weight[8];

void main()
{
  FragPos =vec3(model* vec4(in_pos, 1.f));
   norm =normalize(transpose(inverse(mat3(model))) *a_norm);
   tex_coord = a_tex_coord;
   gl_Position = projection* camera* vec4(FragPos, 1.f);

   for(int i=0;i<4;i++){
    boneId[i]=a_id_bone1[i];
    weight[i] = a_weight1[i];
   }
   for(int i=0;i<4;i++){
    boneId[i+4]=a_id_bone2[i];
    weight[i+4] = a_weight2[i];
   }
}
