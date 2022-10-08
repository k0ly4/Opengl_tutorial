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


struct DirLight{
    vec3 direction;
    vec3 color;
    mat4 spaceMatrix;
    sampler2D shadow_map;
};
uniform DirLight d_light;

in vec2 TexCoord;

float ShadowCalculation(vec3 FragPos,float angle)
{
    
    vec4 fragPosLightSpace = d_light.spaceMatrix*vec4(FragPos,1.f);
    // Выполняем деление перспективы
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
	
    // Трансформируем в диапазон [0,1]
    projCoords = projCoords * 0.5 + 0.5;
	// Оставляем значение тени на уровне 0.0 за границей дальней плоскости пирамиды видимости глазами источника света
    if(projCoords.z > 1.0)
        return 0.f;
    // Получаем наиболее близкое значение глубины исходя из перспективы глазами источника света (используя в диапазон [0,1] fragPosLight в качестве координат)
    float closestDepth = texture(d_light.shadow_map, projCoords.xy).r; 
	
    // Получаем глубину текущего фрагмента исходя из перспективы глазами источника света
    float currentDepth = projCoords.z;
	
    // Вычисляем смещение (на основе разрешения карты глубины и наклона)
    float bias = max(0.05 * (1.0 - angle), 0.005);
	
    // PCF
    float shadow = 0.0;
    vec2 texelSize = 1.0 / textureSize(d_light.shadow_map, 0);
    for(int x = -1; x <= 1; ++x)
    {
        for(int y = -1; y <= 1; ++y)
        {
            float pcfDepth = texture(d_light.shadow_map, projCoords.xy + vec2(x, y) * texelSize).r; 
            shadow += currentDepth - bias > pcfDepth  ? 1.0 : 0.0;        
        }    
    }
    return shadow/9.0f;
}
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

         float shadow = ShadowCalculation(FragPos,dot(Normal,d_light.direction));                    

        lighting +=  (1.0 - shadow)* (diffuse+ vec3(Specular*spec));


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





