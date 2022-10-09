#version 330 core

out vec4 FragColor;
//GBuffer
uniform sampler2D gPosition;//0
uniform sampler2D gNormal;//1
uniform sampler2D gAlbedoSpec;//2
//uniform
uniform vec3 viewPos;
uniform float ambientFactor;
uniform int debugMode;
uniform vec2 gScreenSize;

//PointLight-------------------------------------------------------
const int MAX_POINT_LIGHT = 1;

struct PointLight{
       vec3 position;
       vec3 color;
       vec2 attenuation;
       samplerCube map;
       float far_plane;
};
uniform PointLight p_light[MAX_POINT_LIGHT];
/////factorCubeShadow---------------------
float factorCubeShadow(PointLight light,vec3 FragPos,float angle){

    vec3 fragToLight = FragPos - light.position;
    float currentDepth = length(fragToLight);

    const int samples  = 20;
    vec3 sampleOffsetDirections[samples] = vec3[]
    (
        vec3( 1,  1,  1), vec3( 1, -1,  1), vec3(-1, -1,  1), vec3(-1,  1,  1), 
        vec3( 1,  1, -1), vec3( 1, -1, -1), vec3(-1, -1, -1), vec3(-1,  1, -1),
        vec3( 1,  1,  0), vec3( 1, -1,  0), vec3(-1, -1,  0), vec3(-1,  1,  0),
        vec3( 1,  0,  1), vec3(-1,  0,  1), vec3( 1,  0, -1), vec3(-1,  0, -1),
        vec3( 0,  1,  1), vec3( 0, -1,  1), vec3( 0, -1, -1), vec3( 0,  1, -1)
    );  

    float shadow = 0.0;
    float bias = max(0.06 * (1.0 - angle), 0.005);
    float diskRadius = (1.0 + (currentDepth / light.far_plane)) / 25.0;  
    for(int i = 0; i < samples; ++i)
    {
        float closestDepth = texture(light.map, fragToLight + sampleOffsetDirections[i] * diskRadius).r;
        closestDepth *= light.far_plane;   // undo mapping [0;1]
        if(currentDepth - bias > closestDepth)
            shadow += 1.0;
    }
    shadow /= float(samples);
    return shadow;
}
/////debugCubeShadow---------------------
float debugCubeShadow(PointLight light,vec3 FragPos){
    vec3 fragToLight = FragPos - light.position;
    float closestDepth = texture(light.map, fragToLight).r;
    return closestDepth;
}

/////CalcTexCoord--------------------
vec2 CalcTexCoord()
{
   return gl_FragCoord.xy / gScreenSize;
}
/////main-------------------------------------------------
void main()
{     
    //GBuffer values
    vec2 TexCoord = CalcTexCoord();
    vec3 FragPos = texture(gPosition, TexCoord).rgb;
    vec3 Normal = texture(gNormal, TexCoord).rgb;
    vec3 Albedo = texture(gAlbedoSpec, TexCoord).rgb;
    float Specular = texture(gAlbedoSpec, TexCoord).a; 
    //Final color without alpha channel
    vec3 lighting = Albedo * ambientFactor; // фоновая составляющая
    vec3 viewDir = normalize(viewPos - FragPos);   
    //PointLights-----------------------------------------
    for(int i = 0; i < MAX_POINT_LIGHT; i++)
    {    
        //Затухание
        float distance = length(p_light[i].position-FragPos);
        float attenuation = 1.f /(distance*distance*p_light[i].attenuation.x + distance*p_light[i].attenuation.y+ 1.f);
        // Диффузная составляющая
        vec3 lightDir = normalize(p_light[i].position - FragPos);
        float dotNormal_Dir = dot( lightDir,Normal);

        vec3 diffuse = max(dotNormal_Dir, 0.0) * Albedo * p_light[i].color;
       // Отраженная составляющая
        float spec = pow(max(dot(Normal, normalize(lightDir + viewDir)), 0.0), 16.0);
        //Shadow factor
        float shadow =factorCubeShadow(p_light[i], FragPos,    dotNormal_Dir);      
        //Final factor 
        lighting += (1.f-shadow)*(diffuse+ vec3(Specular*spec))*attenuation;
    }
    //output color
    FragColor = vec4(lighting, 1.0);
    if(debugMode ==1)
        FragColor = vec4(vec3(debugCubeShadow(p_light[0],FragPos)), 1.0);  
}





