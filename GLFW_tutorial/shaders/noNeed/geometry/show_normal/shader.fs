#version 330 core

out vec4 FragColor;
uniform vec3 color_norm;
void main()
{
       FragColor = vec4(color_norm,1.f);
}