#version 330 core

out vec4 FragColor;
in vec2 TexCoord;
//GBuffer
uniform sampler2D gPosition;//0
uniform sampler2D gNormal;//1
uniform sampler2D gAlbedoSpec;//2
//uniform
uniform vec3 viewPos;
uniform float ambientFactor;
uniform mat4 gWVP;
uniform int debugMode;

//DirectionLight----------------------------------------------------------
const int NUM_CASCADES= 5;
struct DirLight{
    vec3 direction;
    vec3 color;
    mat4 spaceMatrix[NUM_CASCADES];
    sampler2DArray map;
    float cascadePlaneDistances[NUM_CASCADES+1];
};
uniform DirLight d_light;

//Get cascade level from cascade planes distance and cur frag position
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
int cascade_level;

//If debug mode
vec3 debugColorCascade(int level){
    const vec3 color_shadow[NUM_CASCADES+1] = {
            vec3(1.f,0.f,0.f),
            vec3(1.f,1.f,0.f),
            vec3(0.f,1.f,0.f),
            vec3(0.f,1.f,1.f),
            vec3(0.f,0.f,1.f),
            vec3(0.f,0.f,0.f)
       };
    return color_shadow[level];
}

//Calculate float factor of cascade shadow for direction light
//angle =dot(normal, lightDir)
float factorCascadeShadow(vec4 FragPos,float angle){
    cascade_level = cascadeLevel(gWVP * FragPos);

    vec4 fragPosLightSpace  = d_light.spaceMatrix[cascade_level] * FragPos;
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
    bias *= 1.f / (d_light.cascadePlaneDistances[cascade_level] * 0.5f);

    // PCF

    float shadow = 0.0;
    vec2 texelSize = 1.0 / vec2(textureSize(d_light.map, 0));

    const int HALF_WIGHT = 3;

    for(int x = -HALF_WIGHT; x <= HALF_WIGHT; ++x)
    {
	    for(int y = -HALF_WIGHT; y <= HALF_WIGHT; ++y)
	    {
		    float pcfDepth = texture(d_light.map, vec3(projCoords.xy + vec2(x, y) * texelSize, cascade_level)).r;
		    shadow += (currentDepth- bias) > pcfDepth ? 1.0 : 0.0;
	    }
    }
    
    shadow /= ((HALF_WIGHT*2+1)*(HALF_WIGHT*2+1)); 
    return shadow;
}


void main()
{     
    //GBuffer values
    vec3 FragPos = texture(gPosition, TexCoord).rgb;
    vec3 Normal = texture(gNormal, TexCoord).rgb;
    vec3 Albedo = texture(gAlbedoSpec, TexCoord).rgb;
    float Specular = texture(gAlbedoSpec, TexCoord).a; 
    //Final color without alpha channel
    vec3 lighting = Albedo * ambientFactor; // фоновая составляющая
    //DirectionLight-----------------------------------
        float dotNormal_Dir = dot(Normal, d_light.direction);
        //Направленный свет
        vec3 diffuse = max(dotNormal_Dir, 0.0) * Albedo * d_light.color;
        // Отраженная составляющая
        vec3 viewDir = normalize(viewPos - FragPos);
        float spec = pow(max(dot(Normal, normalize(d_light.direction + viewDir)), 0.0), 16.0);
        //Shadow factor
        float shadow = factorCascadeShadow(vec4(FragPos,1.f), dotNormal_Dir);                    
        
        lighting +=  (1.0 - shadow)*(diffuse + vec3(Specular*spec)) *(debugMode ==1 ? debugColorCascade(cascade_level): vec3(1.f)); //debug

    FragColor = vec4(lighting, 1.0);
}





