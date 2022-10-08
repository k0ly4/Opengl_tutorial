#version 330 core

out vec4 FragColor;

in vec3 norm;
in vec3 FragPos;
in vec2 texCoord;
in vec4 fragPosLightSpace;

struct Material{
    sampler2D 
    diffuse,
    specular;

    float shininess;
};

uniform Material material;

uniform vec3 viewPos;
uniform sampler2D shadowMap;


struct DirLight{
    vec3 
        direction,
        ambient,
        diffuse,
        specular;
};
uniform DirLight dirLight;

float getShadow(float angle){
    vec3 pojCoords = fragPosLightSpace.xyz/fragPosLightSpace.w;
    pojCoords = pojCoords*0.5f+0.5f;
    if(pojCoords.z>1.f)return 1.f;
    float shadow =0.f;
    vec2 texel_size = 1.f/textureSize(shadowMap,0);
    float clossestDepth = pojCoords.z- max(0.05f*(1.f-angle),0.005f);

    for(int  x = -1;x<2;++x){
        for(int y =-1;y<2;++y){
            float depth = texture(shadowMap,pojCoords.xy+vec2(x,y)*texel_size).r;
            if(depth>clossestDepth)shadow+=1.f;
        }
    }
    return shadow/9.f;
}
vec3 factorDirLight(DirLight light, vec3 text_color, vec3 viewDir){
    vec3 lightDir = normalize(-light.direction);
    float angle = dot(norm,lightDir);
    float diff = max(angle,0.f);

    float spec = pow(max(dot(norm,normalize(lightDir+viewDir)),0.f),material.shininess);

    float shadow = getShadow(angle);
    vec3 result = light.ambient;
    result += light.diffuse *diff*shadow;
    result += light.specular* spec*shadow;
    return result*text_color;
}

struct PointLight{
    vec3 
        position,
        factor,
        ambient,
        diffuse,
        specular;
};

uniform PointLight pointLight;

vec3 factorPointLight(PointLight light, vec3 text_color, vec3 viewDir){
    vec3 lightDir = normalize (light.position - FragPos);
    float angle_diff = dot(norm,lightDir);

    float distance = length(light.position-FragPos);
   
    float spec = pow(max(dot(norm,normalize(lightDir+viewDir)),0.f),material.shininess);
    
    float attenuation = 1.f/ (light.factor.x + light.factor.y*distance+ light.factor.z*distance*distance);

    float shadow = getShadow(angle_diff);

    vec3 result = light.ambient ;
    result+= light.diffuse*max(angle_diff,0.f)*shadow;
    result+= light.specular* spec*shadow;
    return result *attenuation*text_color;

}

void main()
{
   vec3 viewDir = normalize(viewPos-FragPos);
   vec3 text_color = vec3(texture(material.diffuse,texCoord));
   vec3 result = vec3(0.f);// =factorDirLight(dirLight,text_color,viewDir);

   result += factorPointLight(pointLight, text_color,viewDir);

    FragColor = vec4(result,1.f);
}