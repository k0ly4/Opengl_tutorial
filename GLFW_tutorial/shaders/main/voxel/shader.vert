#version 330 core

layout (location = 0) in vec3 v_position;
layout (location = 1) in vec2 v_texCoord;
layout (location = 2) in vec4 v_light;

out vec3 a_color;
out vec2 a_texCoord;
out float visibility;
//color
uniform vec3 color_sun;
uniform float ambient;
//mat4
uniform mat4 model;
uniform mat4 camera;
uniform mat4 projection;
//fog
uniform float density;
uniform float gradient;

void main(){
	a_color =v_light.rgb +v_light.a*color_sun + ambient;
	a_texCoord = v_texCoord;
	vec4 posRelView  = camera * model * vec4(v_position, 1.0);
	gl_Position = projection * posRelView;
	//fog
	float distance = length(posRelView.xyz);
	visibility = exp(-pow((distance* density), gradient));
	visibility = clamp(visibility,0.0,1.0);
}