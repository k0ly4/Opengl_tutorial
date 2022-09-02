#version 330 core
layout (location = 0) out vec3 gPosition;
layout (location = 1) out vec3 gNormal;
layout (location = 2) out vec4 gAlbedoSpec;
 
uniform vec3 color;
in vec3 FragPos;
in vec3 norm;

flat in vec4 boneId;
in vec4 weight;
 
uniform int gDisplayBoneIndex;
uniform int boneMode;

uniform float specularMaterial;
void main()
{    
    // Храним вектор позиции фрагмента в первой текстуре G-буфера
    gPosition = FragPos;
    // Также храним нормали каждого фрагмента в G-буфере
    gNormal = norm; 
    vec3 indicator =vec3(0.0);
    for (int i = 0 ; i < 4 ; i++) {
        if (boneId[i] == gDisplayBoneIndex) {
           if (weight[i] >= 0.7) {
               indicator = vec3(1.0, 0.0, 0.0) * weight[i];
           } else if (weight[i] >= 0.4 && weight[i] <= 0.6){ 
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
    else gAlbedoSpec.rgb = color;
    
    // Сохраняем значение интенсивности зеркальной составляющей в альфа-компоненте переменной gAlbedoSpec
    gAlbedoSpec.a = specularMaterial;
}