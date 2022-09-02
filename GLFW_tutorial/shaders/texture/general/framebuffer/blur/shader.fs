#version 330 core
out vec4 FragColor;
in vec2 texCoord;
uniform sampler2D image;

uniform bool horizontal;
const float weight[5] = float [](0.227027, 0.1945946, 0.1216216, 0.054054, 0.016216);

void main()

{     
       vec2 tex_offset = 1.f/textureSize(image,0);
       vec3 result= texture(image,texCoord).rgb*weight[0];
       if(horizontal){
              for(int i=1;i<5;i++){
                     result+=texture(image,texCoord+vec2(tex_offset.x*i,0.f)).rgb*weight[i]; 
                     result+=texture(image,texCoord-vec2(tex_offset.x*i,0.f)).rgb*weight[i];
              }

       }
       else{
                for(int i=1;i<5;i++){
                     result+=texture(image,texCoord+vec2(tex_offset.y*i,0.f)).rgb*weight[i]; 
                     result+=texture(image,texCoord-vec2(tex_offset.y*i,0.f)).rgb*weight[i];
              }

       }
       FragColor = vec4(result,1.f);
}