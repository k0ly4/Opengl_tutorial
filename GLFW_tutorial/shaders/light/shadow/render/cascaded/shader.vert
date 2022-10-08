#version 460 core
layout (location = 0) in vec3 in_pos;

uniform mat4 model;
void main()
{  
   gl_Position =model*vec4(in_pos,1.f);
   //gl_Position.z +=0.01; //  float bias = max(0.05 * (1.0 - dot(normal, lightDir)), 0.005);
}