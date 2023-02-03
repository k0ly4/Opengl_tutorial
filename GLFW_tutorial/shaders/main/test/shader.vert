#version 330 core

layout (location = 0) in vec3 v_position;
layout (location = 1) in vec2 v_texCoord;

out vec2 a_texCoord;

uniform mat4 model;
uniform mat4 projection;
uniform mat4 camera;

void main(){
	
	a_texCoord = v_texCoord;
	gl_Position = (projection* camera*model* vec4(v_position.xy,1.f,1.f)).xyww;
	//gl_Position = VP * model * vec4(v_position, 1.0);
}