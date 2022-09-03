#version 330 core

uniform sampler2D gPosition;//0
uniform sampler2D gNormal;//1
uniform sampler2D gAlbedoSpec;//2
uniform vec3 viewPos;
uniform float ambientFactor;

struct PointLight{
       vec3 position;
       vec3 color;
       vec2 attenuation;
};
const int MAX_POINT_LIGHT = 1;
uniform PointLight p_light[MAX_POINT_LIGHT];

const int NUM_CASCADES= 5;

struct DirLight{
    vec3 direction;
    vec3 color;
    mat4 spaceMatrix[NUM_CASCADES];
    sampler2DArray shadow_map;
    float cascadePlaneDistances[NUM_CASCADES+1];
};
uniform mat4 gWVP;
uniform DirLight d_light;
int cascadeLevel(vec4 fragPosViewSpace){

    float depthValue = abs(fragPosViewSpace.z);
        
    for (int i = 0; i < NUM_CASCADES-1; ++i)
    {
        if (depthValue < d_light.cascadePlaneDistances[i])
        {
        return i;
        }
    }
    return NUM_CASCADES-1;
}
int level;
float factorShadow(vec4 FragPos,float angle){//angle =dot(normal, lightDir)
    level = cascadeLevel(gWVP * FragPos);

    vec4 fragPosLightSpace  = d_light.spaceMatrix[level] * FragPos;
    // perform perspective divide
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    // transform to [0,1] range
    projCoords = projCoords * 0.5 + 0.5;
    // get depth of current fragment from light's perspective
    float currentDepth = projCoords.z;

     if (currentDepth  > 1.0)
     {
         return 0.0;
     }
    // calculate bias (based on depth map resolution and slope)
    float bias = max(0.05 * (1.0 - angle), 0.005);
    
    bias *= 1 / (d_light.cascadePlaneDistances[level] * 0.5f);

    // PCF
    float shadow = 0.0;
    vec2 texelSize = 1.0 / vec2(textureSize(d_light.shadow_map, 0));
    for(int x = -1; x <= 1; ++x)
    {
        for(int y = -1; y <= 1; ++y)
        {
            float pcfDepth = texture(d_light.shadow_map, vec3(projCoords.xy + vec2(x, y) * texelSize, level)).r;
            shadow += (currentDepth - bias) > pcfDepth ? 1.0 : 0.0;        
        }    
    }           
    return shadow/9.0;;
}
in vec2 TexCoord;
out vec4 FragColor;


void main()
{     
    vec3 FragPos = texture(gPosition, TexCoord).rgb;

    vec3 Normal = texture(gNormal, TexCoord).rgb;
    vec3 Albedo = texture(gAlbedoSpec, TexCoord).rgb;
    float Specular = texture(gAlbedoSpec, TexCoord).a; 

    vec3 lighting = Albedo * ambientFactor; // фоновая составляющая

    //Направленный свет
        vec3 diffuse = max(dot(Normal, d_light.direction), 0.0) * Albedo * d_light.color;
       // Отраженная составляющая
       vec3 viewDir = normalize(viewPos - FragPos);
       float spec = pow(max(dot(Normal, normalize(d_light.direction + viewDir)), 0.0), 16.0);
       const vec3 color_shadow[NUM_CASCADES+1] = {
            vec3(1.f,0.f,0.f),
            vec3(1.f,1.f,0.f),
            vec3(0.f,1.f,0.f),
            vec3(0.f,1.f,1.f),
            vec3(0.f,0.f,1.f),
            vec3(0.f,0.f,0.f)
       };
        
         float shadow = factorShadow(vec4(FragPos,1.f),dot(normalize(Normal),d_light.direction));                    

        lighting +=  (1.0 - shadow)*(diffuse+ vec3(Specular*spec))* color_shadow[level];


        for(int i = 0; i < MAX_POINT_LIGHT; ++i)
    {    //Затухание
            float distance = length(p_light[i].position-FragPos);
            float attenuation = 1.f /(distance*distance*p_light[i].attenuation.x + distance*p_light[i].attenuation.y+ 1.f);
        // Диффузная составляющая
         vec3 lightDir = normalize(p_light[i].position - FragPos);
         diffuse = max(dot(Normal, lightDir), 0.0) * Albedo * p_light[i].color;
       // Отраженная составляющая
        spec = pow(max(dot(Normal, normalize(lightDir + viewDir)), 0.0), 16.0);
       
        lighting += (diffuse+ vec3(Specular*spec))*attenuation;
    }
    
    FragColor = vec4(lighting, 1.0);
}





