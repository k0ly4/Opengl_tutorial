#version 330 core

layout (location = 0) in vec3 in_pos;
layout (location =1) in vec3 a_norm;

layout (location =3) in ivec4 a_id_bone1;
layout (location = 4) in vec4 a_weight1;


uniform mat4 model;
uniform mat4 camera;
uniform mat4 projection;

const int MAX_BONES = 100;
uniform mat4 bones[MAX_BONES];

out vec3 norm;
out vec3 FragPos;
flat out vec4 boneId;
out vec4 weight;

void main()
{
    mat4 boneTransform =bones[a_id_bone1[0]]*a_weight1[0];
    boneTransform += bones[a_id_bone1[1]]*a_weight1[1];
    boneTransform += bones[a_id_bone1[2]]*a_weight1[2];
    boneTransform += bones[a_id_bone1[3]]*a_weight1[3];
    
    boneId=a_id_bone1;
    weight = a_weight1;
  
  //boneTransform =model* boneTransform;
  FragPos =vec3(boneTransform* vec4(in_pos, 1.f));
  norm =normalize(transpose(inverse(mat3(boneTransform))) *a_norm);
  gl_Position = projection* camera* vec4(FragPos, 1.f);

  
}
