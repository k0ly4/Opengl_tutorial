#version 330 core

uniform sampler2D image1;
uniform sampler2D image2;

in vec2 texCoord;
uniform float exposure;
out vec4 FragColor;

void main()

{     
       const float gamma = 2.2f;
        vec3 hdrColor = texture(image1,texCoord).rgb + texture(image2,texCoord).rgb;
        hdrColor =  vec3(1.f) - exp(-hdrColor* exposure);
       hdrColor = pow(hdrColor,vec3(1.f/gamma));
       FragColor =vec4(hdrColor,1.f);
}