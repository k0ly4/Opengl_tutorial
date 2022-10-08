#version 330 core
out vec4 FragColor;
  
in vec2 texCoord;
 
uniform sampler2D image;
 
void main()
{             
    FragColor = vec4(vec3(texture(image, texCoord).r), 1.0);
    //FragColor = vec4(1.f);
}  