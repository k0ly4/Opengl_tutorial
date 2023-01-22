#version 330 core

uniform sampler2D diffuse;
uniform vec4 color = vec4(1);

in vec2 uv;
out vec4 FragColor;

void main()
{      
      FragColor =texture(diffuse,uv)*color;
}