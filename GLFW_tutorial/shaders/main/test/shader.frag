#version 330 core

in vec2 a_texCoord;
out vec4 FragColor;

uniform vec4 color;
uniform sampler2D diffuse;

void main(){
	FragColor = texture(diffuse, a_texCoord)*color;
	//FragColor.a = (FragColor.r+FragColor.g)/2.f;
}