#version 330 core

uniform samplerCube skybox;
uniform vec3 camera_pos;

in vec3 norm;
in vec3 pos;

out vec4 FragColor;

void main()
{
         float ratio = 1.f/1.9f;
       vec3 R = refract(normalize(pos-camera_pos),norm,ratio);
       FragColor =vec4(texture(skybox,R).rgb,1.f);
}