#version 330 core
struct Material{
sampler2D diffuse1;
};
uniform Material material;
in vec2 texCoord;

out vec4 FragColor;

void main()
{
       FragColor =texture( material.diffuse1,texCoord);
}