#version 330 core

uniform sampler2D image;

in vec2 texCoord;
out float  FragColor;

void main()
{    
       FragColor = texture(image,texCoord).r;
}