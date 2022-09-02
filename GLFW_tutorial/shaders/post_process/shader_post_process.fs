#version 330 core

uniform sampler2D texture_diffuse1;

in vec2 texCoord;

out vec4 FragColor;

const float offset = 1.f/300.f;

const vec2 offsets[9] = vec2[](
            vec2(-offset,offset),
            vec2(0.f,offset),
            vec2(offset,offset),
            vec2(-offset,0.f),
            vec2(0.f,0.f),
            vec2(offset,0.f),
            vec2(-offset,-offset),
            vec2(0.f,-offset),
            vec2(offset,-offset)
);
const float kernel[9] = float[](
            1,1,1,
            1,-8,1,
            1,1,1
        );
void main()
{         
        
          vec3 col = vec3(0.f);
        for(int i=0;i<9;i++)
        {   
            col += texture(texture_diffuse1,texCoord.st+ offsets[i]).rgb*kernel[i];

        }  
       FragColor =vec4( col,1.f);
}