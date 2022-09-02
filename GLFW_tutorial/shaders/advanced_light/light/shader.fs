#version 330 core

layout (location =0) out vec4 FragColor;
layout (location =1) out vec4 BrightColor;

in vec2 texCoord; 

struct Light{
    vec3 
        position,
        color;
    float specular;
};
uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D gAlbedoSpec;
uniform sampler2D ssao;

uniform float shininess;
uniform float ambient;

//const int NR_LIGHTS = 1;
uniform Light light;//[NR_LIGHTS];

void main()
{
    vec3 fragPos = texture(gPosition,texCoord).rgb;
    vec3 Normal = texture(gNormal,texCoord).rgb;
    vec4 albedo = texture(gAlbedoSpec,texCoord);
    vec3 result = albedo.rgb*ambient;//*texture(ssao,texCoord).r;

    vec3 viewDir = normalize(-fragPos);
   // for(int i=0;i< NR_LIGHTS;i++){
        float distance = length(light.position-fragPos);
        float attenuation = 1.f /(distance*distance);

        vec3 lightDir = normalize(light.position - fragPos);
        result += max(dot(Normal,lightDir),0.f)*albedo.rgb*light.color*attenuation;

        float spec = pow(max(dot(Normal,normalize(lightDir+viewDir)),0.f),shininess);
      //  result +=spec*light.specular*light.color*attenuation*albedo.a;
    //}

    FragColor = vec4(result,1.f);

    float brightness = dot(result,vec3(0.2126,0.7152,0.0722));
    if(brightness>1.f) BrightColor = FragColor;
    else  BrightColor = vec4(0.f,0.f,0.f,1.f);
}