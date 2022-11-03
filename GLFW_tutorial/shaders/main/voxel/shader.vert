#version 330 core

layout (location = 0) in vec3 v_position;
layout (location = 1) in vec2 v_texCoord;
layout (location = 2) in vec4 v_light;

out vec4 a_color;
out vec2 a_texCoord;

uniform mat4 model;
uniform mat4 VP;

void main(){
	a_color = vec4(v_light.rgb +v_light.a,1.0f);
	a_texCoord = v_texCoord;
	gl_Position = VP * model * vec4(v_position, 1.0);
}