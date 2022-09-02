#version 330 core

uniform sampler2D image;
uniform float exposure;

in vec2 texCoord;
out vec4 FragColor;

void main()
{     
       const float gamma = 2.2f;
        vec3 hdrColor = texture(image,texCoord).rgb;
        hdrColor =  vec3(1.f) - exp(-hdrColor* exposure);
       hdrColor = pow(hdrColor,vec3(1.f/gamma));
       FragColor =vec4(hdrColor,1.f);
}