#version 330 core

out vec4 FragColor;


in vec3 norm;
in vec3 FragPos;
in vec2 texCoord;

struct Material{
    sampler2D 
    diffuse,
    specular;

    float shininess;
};

uniform Material material;
uniform vec3 viewPos;

struct DirLight{
    vec3 
        direction,
        ambient,
        diffuse,
        specular;
};
uniform DirLight dirLight;

vec3 factorDirLight(DirLight light,vec3 normal,vec3 viewDir){
    vec3 lightDir = normalize(-light.direction);
    float diff = max(dot(normal,lightDir),0.f);

    vec3 reflectDir = reflect(-lightDir,normal);
    float spec = pow(max(dot(viewDir,reflectDir),0.f),material.shininess);

    vec3 result = light.ambient* vec3(texture(material.diffuse,texCoord));
    result += light.diffuse *diff* vec3(texture(material.diffuse,texCoord));
    result += light.specular* spec* vec3(texture(material.diffuse,texCoord));
    return result;
}

struct PointLight{
    vec3 
        position,
        factor,
        ambient,
        diffuse,
        specular;
};

#define NR_POINT_LIGHTS 4

uniform PointLight pointLight[NR_POINT_LIGHTS];



vec3 factorPointLight(PointLight light,vec3 normal, vec3 fragPos, vec3 viewDir){
    vec3 lightDir = normalize (light.position - fragPos);
    float diff = max(dot(normal,lightDir),0.f);

    vec3 reflectDir = reflect(-lightDir,normal);
    float spec = pow(max(dot(viewDir,reflectDir),0.f),material.shininess);

    float distance = length(light.position-fragPos);
    float attenuation = 1.f / (light.factor.x + light.factor.y*distance+ light.factor.z*distance*distance);

    vec3 result = light.ambient* vec3(texture(material.diffuse,texCoord));
    result+= light.diffuse*diff*vec3(texture(material.diffuse,texCoord));
    result+= light.specular* spec*vec3(texture(material.specular,texCoord));
    return result *attenuation;

}

struct SpotLight{
    vec3
        position,
        ambient,
        diffuse,
        specular,
        factor,
        direction;
    float 
        cut_off,
        outer_cut_off;
};

uniform SpotLight spotLight;

vec3 factorSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir){
    vec3 lightDir = normalize(light.position - FragPos);
    float diff = max(dot(normal, lightDir),0.f);

    vec3 reflectDir = reflect(-lightDir,norm);
    float spec = pow(max(dot(viewDir,reflectDir),0.f),material.shininess);

    float distance = length (light.position-fragPos);
    float attenuation = 1.f/(light.factor.x + light.factor.y*distance+ light.factor.z*distance*distance);

 float theta = dot(lightDir,normalize(-light.direction));
    float itensity = clamp((theta- light.outer_cut_off)/(light.cut_off - light.outer_cut_off),0.f,1.f);

     vec3 result = light.ambient* vec3(texture(material.diffuse,texCoord));
    result+= itensity*light.diffuse*diff*vec3(texture(material.diffuse,texCoord));
    result+= itensity*light.specular* spec*vec3(texture(material.specular,texCoord));
   
    
    return attenuation* result;
}

void main()
{
   vec3 viewDir = normalize(viewPos-FragPos);

   vec3 result = factorDirLight(dirLight,norm,viewDir);

   for(int i=0;i<NR_POINT_LIGHTS;i++)
    result += factorPointLight(pointLight[i],norm,FragPos,viewDir);

    result += factorSpotLight(spotLight,norm, FragPos,viewDir);

    FragColor = vec4(result,1.f);
}