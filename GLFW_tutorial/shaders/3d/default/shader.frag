#version 330 core


in vec2 texCoord;
out vec4 FragColor;

//Material-------------------------------------
uniform int configMaterial;
uniform sampler2D diffuse;
uniform vec4 baseColor;

void main()
{
   
      FragColor =configMaterial == 1? texture(diffuse,texCoord)*baseColor:baseColor;
      if(FragColor.a<0.1f)
            discard;
}