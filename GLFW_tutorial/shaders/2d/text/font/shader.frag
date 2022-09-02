#version 330 core

uniform sampler2D glyph;

in vec2 texCoord;
out float  FragColor;

void main()
{    
       FragColor = texture(glyph,texCoord).r;
}