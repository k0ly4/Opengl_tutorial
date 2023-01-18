#version 330 core

uniform sampler2D diffuse1;
uniform vec4 color = vec4(1.f);

in vec2 texCoord;
out vec4 FragColor;

void main()
{
       FragColor =texture(diffuse1,texCoord)*color;
	   if(FragColor.a<0.1)discard;
}