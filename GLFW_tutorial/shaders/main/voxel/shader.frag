#version 330 core

in vec4 a_color;
in vec2 a_texCoord;
out vec4 FragColor;

uniform sampler2D diffuse;

void main(){
	FragColor = a_color * texture(diffuse, a_texCoord);
}