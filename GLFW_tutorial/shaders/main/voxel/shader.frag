#version 330 core

out vec4 FragColor;

in vec3 a_color;
in vec2 a_texCoord;
in float visibility;

uniform sampler2D diffuse;
uniform vec3 skyColor;

void main(){
	vec4 tex_color = texture(diffuse, a_texCoord);
	if (tex_color.a < 0.2) discard;
	FragColor = vec4(a_color,1.0) * tex_color;
	//fog
	FragColor = mix(vec4(skyColor,1.0),FragColor,visibility);
}