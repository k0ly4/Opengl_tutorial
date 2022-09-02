#version 330 core
layout (location = 0) out vec3 gPosition;
layout (location = 1) out vec3 gNormal;
layout (location = 2) out vec4 gAlbedoSpec;
 
in vec2 tex_coord;
in vec3 FragPos;
in vec3 norm;

flat in float boneId[8];
in float weight[8];
 
uniform int gDisplayBoneIndex;
uniform int boneMode;

uniform sampler2D diffuse1;
uniform float specularMaterial;
void main()
{    
    // Храним вектор позиции фрагмента в первой текстуре G-буфера
    gPosition = FragPos;
    // Также храним нормали каждого фрагмента в G-буфере
    gNormal = norm; 
    vec3 indicator =vec3(0.0);
    for (int i = 0 ; i < 8 ; i++) {
        if (boneId[i] == gDisplayBoneIndex) {
           if (weight[i] >= 0.7) {
               indicator = vec3(1.0, 0.0, 0.0) * weight[i];
           } else if (weight[i] >= 0.4 && weight[i] <= 0.6) {
               indicator = vec3(0.0, 1.0, 0.0) * weight[i];
           } else if (weight[i] >= 0.1) {
               indicator = vec3(1.0, 1.0, 0.0) * weight[i];
           }
           else indicator = vec3(0.0, 0.0, 1.0);
           break;
        }
    }
    // И диффузную составляющую цвета каждого фрагмента
    if(boneMode) gAlbedoSpec.rgb = indicator;
    else gAlbedoSpec.rgb = texture(diffuse1, tex_coord).rgb;
    
    // Сохраняем значение интенсивности зеркальной составляющей в альфа-компоненте переменной gAlbedoSpec
    gAlbedoSpec.a = specularMaterial;
}