#version 330 core

out vec4 FragColor;

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoord;
in vec4 FragPosView;
//Uniform------------------------------------------------
uniform vec3 viewPos;
uniform float ambientFactor;
uniform int debugMode;
const float shininess = 16.0;
//Material------------------------------------------------
uniform int configMaterial;
uniform vec4 baseColor;
uniform sampler2D diffuse;
uniform float specularMaterial;
//DirectionLight----------------------------------------------------------
const int NUM_CASCADES= 5;
struct DirLight{
    vec3 direction;
    vec3 color;
    mat4 spaceMatrix[NUM_CASCADES];
    sampler2DArray map;
    float cascadePlaneDistances[NUM_CASCADES+1];
    int enable;
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
//Calculate float factor of cascade shadow for direction light
//angle =dot(normal, lightDir)
float factorCascadeShadow(vec4 FragPos,float angle){
    cascade_level = cascadeLevel(FragPosView);

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
    vec4 materialColor = configMaterial == 1 ? texture(diffuse, TexCoord) * baseColor : baseColor;
    vec3 Albedo = materialColor.rgb;
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 lighting = Albedo * ambientFactor; // фоновая составляющая
    //DirectionLight-----------------------------------
    float dotNormal_Dir = dot(Normal, d_light.direction);
    // Диффузная составляющая
    vec3 diffuseFactor = max(dotNormal_Dir, 0.0) * Albedo * d_light.color;
    // Отраженная составляющая
    float specularFactor = specularMaterial * pow(max(dot(Normal, normalize(d_light.direction + viewDir)), 0.0), shininess);
    ////Shadow factor
    float shadow =factorCascadeShadow(vec4(FragPos,1.f), dotNormal_Dir);
    //Final factor 
    lighting += (1.0 - shadow) *(diffuseFactor+ vec3(specularFactor));

    FragColor = vec4(lighting, materialColor.a);
}





