#version 330 core

uniform sampler2D image;
uniform float exposure;

in vec2 texCoord;
out vec4 FragColor;

vec3 getHDR(vec3 color){
        const float gamma = 2.2f;
       color =  vec3(1.f) - exp(-color* exposure);
       return pow(color,vec3(1.f/gamma));
}

void main()
{     
       vec4 imageColor = texture(image,texCoord);
       FragColor =vec4(getHDR(imageColor.rgb),imageColor.a);
}
