#version 330 core

layout (location = 0) in vec4 vertex;


uniform mat4 model;
uniform mat4 model_uv;
uniform mat4 projection;

out vec2 texCoord;

void main()
{  
    texCoord =vec2(model_uv* vec4(vertex.zw,0.f,1.f));
   gl_Position =projection* model* vec4(vertex.xy,0.f,1.f);
}