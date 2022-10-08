#version 330 core
uniform sampler2D depthMap;
out vec4 FragColor;
in vec2 texCoord;
void main()
{
    FragColor = vec4(vec3(texture(depthMap,texCoord).r),1.f);
}