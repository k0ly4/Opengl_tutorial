#version 330 core

uniform sampler2D image;

in vec2 texCoord;

out vec4 FragColor;

void main()

{     
      
        float hdrColor = texture(image,texCoord).r;
       
       FragColor =vec4(hdrColor,hdrColor,hdrColor,1.f);
}