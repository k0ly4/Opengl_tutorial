#version 330 core

in vec4 a_color;
in vec2 a_texCoord;
out vec4 FragColor;

uniform sampler2D diffuse;

void main(){
	vec4 tex_color = texture(diffuse, a_texCoord);
	if (tex_color.a < 0.5)
		discard;
	FragColor = a_color * tex_color;
}