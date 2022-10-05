#version 330 core

uniform sampler2D glyph;
uniform vec4 color;

in vec2 texCoord;
out vec4 FragColor;

void main()
{      
       FragColor =vec4(color.rgb,color.a*texture(glyph,texCoord));
}