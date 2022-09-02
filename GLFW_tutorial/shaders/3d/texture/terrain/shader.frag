#version 330 core

out vec4 FragColor;
in vec3 color;
in vec3 normal;
void main()
{
   
       FragColor = vec4((normal+1.f)/2.f,1.f);
}