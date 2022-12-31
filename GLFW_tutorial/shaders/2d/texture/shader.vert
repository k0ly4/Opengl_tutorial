#version 330 core

layout (location = 0) in vec4 vertex;

//uniform mat4 view;
uniform mat4 projection;
uniform mat4 model;
out vec2 uv;

void main()
{  
	uv = vertex.zw;
	gl_Position =projection* model* vec4(vertex.xy,0.f,1.f);
}