#version 330 core

out vec4 FragColor;

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoord;
//Uniform------------------------------------------------
uniform vec3 viewPos;
uniform float ambientFactor;
const float shininess = 16.0;
//Material------------------------------------------------
uniform int configMaterial;
uniform vec4 baseColor;
uniform sampler2D diffuse;
uniform float specularMaterial;
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
float factorCubeShadow(PointLight light,float angle)
{
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

void main()
{        
    vec4 materialColor = configMaterial == 1 ? texture(diffuse, TexCoord) * baseColor : baseColor;
    vec3 Albedo = materialColor.rgb;

    
    vec3 viewDir = normalize(viewPos - FragPos);

    vec3 lighting = Albedo * ambientFactor; // фоновая составляющая
    //attenuation
      for(int i = 0; i < MAX_POINT_LIGHT; i++)
    {    
        //Затухание
        float distance = length(p_light[i].position-FragPos);
        float attenuation = 1.f /(distance*distance*p_light[i].attenuation.x + distance*p_light[i].attenuation.y+ 1.f);

        // Диффузная составляющая
        vec3 lightDir = normalize(p_light[i].position - FragPos);
        float dotNormal_Dir = dot(Normal, lightDir);

        vec3 diffuseFactor = max(dotNormal_Dir, 0.0) * Albedo * p_light[i].color;
       // Отраженная составляющая
        float specularFactor = specularMaterial * pow(max(dot(Normal, normalize(lightDir + viewDir)), 0.0), shininess);

        ////Shadow factor
        float shadow =factorCubeShadow(p_light[i],dotNormal_Dir);
             
        //Final factor 
        lighting += (1.0 - shadow) *(diffuseFactor+ vec3(specularFactor))*attenuation;
    }

    FragColor = vec4(lighting,materialColor.a);
}


