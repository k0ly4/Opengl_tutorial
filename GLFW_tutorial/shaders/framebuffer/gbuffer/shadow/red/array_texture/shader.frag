#version 330 core
out vec4 FragColor;
  
in vec2 texCoord;
 
uniform sampler2DArray image;
uniform int level;
void main()
{             
    FragColor = vec4(vec3(texture(image, vec3(texCoord,level)).r), 1.0);
}  