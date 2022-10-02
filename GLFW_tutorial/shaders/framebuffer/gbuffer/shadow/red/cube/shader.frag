#version 330 core

uniform samplerCube cube;
uniform float far_plane;

in vec3 texCoord;
out vec4 FragColor;


void main()
{
       float depth = texture(cube,texCoord).r;
       FragColor = vec4(vec3(depth / far_plane), 1.0);  
}