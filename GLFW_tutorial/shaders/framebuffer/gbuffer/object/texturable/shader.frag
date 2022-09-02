#version 330 core
layout (location = 0) out vec3 gPosition;
layout (location = 1) out vec3 gNormal;
layout (location = 2) out vec4 gAlbedoSpec;
 
in vec2 tex_coord;
in vec3 FragPos;
in vec3 norm;

 
uniform sampler2D diffuse1;
uniform float specularMaterial;
 
void main()
{    
    // Храним вектор позиции фрагмента в первой текстуре G-буфера
    gPosition = FragPos;
    // Также храним нормали каждого фрагмента в G-буфере
    gNormal = norm; 
    // И диффузную составляющую цвета каждого фрагмента
    gAlbedoSpec.rgb = texture(diffuse1, tex_coord).rgb;
    // Сохраняем значение интенсивности зеркальной составляющей в альфа-компоненте переменной gAlbedoSpec
    gAlbedoSpec.a = specularMaterial;
}