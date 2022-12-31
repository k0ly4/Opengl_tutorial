#version 330 core

uniform sampler2D diffuse;
uniform vec4 color;

in vec2 uv;
out vec4 FragColor;

void main()
{      
      FragColor =texture(diffuse,uv)*color;
}