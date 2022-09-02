#version 330 core

layout (location = 0) in vec4 vertex;


uniform  mat4 projection;

out vec2 texCoord;

void main()
{  
    texCoord = vertex.zw;
   gl_Position = projection* vec4(vertex.xy,0.f,1.f);
}