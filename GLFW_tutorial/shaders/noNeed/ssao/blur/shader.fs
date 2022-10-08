#version 330 core

uniform sampler2D image;

in vec2 texCoord;

out float FragColor;

void main()

{     
      vec2 texelSize = 1.f/vec2(textureSize(image,0));
      float result = 0.f;
      for(int x = -2;x<2;x++)
        for(int y=-2;y<2;y++){
                vec2 offset = vec2(float(x),float(y))*texelSize;
                result+=texture(image,texCoord+offset).r;
        }

       FragColor =result/16.f;
}