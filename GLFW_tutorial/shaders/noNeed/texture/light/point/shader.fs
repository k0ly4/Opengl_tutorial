#version 330 core

out vec4 FragColor;

in VS_OUT{
    vec2 texCoord;
    vec3  norm;
    vec3 FragPos;
}fs_in;

struct Material{
    sampler2D 
    diffuse1,
    specular1;

    float shininess;
};
struct PointLight{
    vec3 
        position,
        factor,
        ambient,
        diffuse,
        specular;
};
const int size_pointLight = 4;
uniform PointLight pointLight[size_pointLight];
uniform Material material;
uniform vec3 viewPos;
uniform float far_plane;
uniform samplerCube shadowMap;

float shadowCalcutation(vec3 lightPos,float angle){
    vec3 fragToLight = fs_in.FragPos-lightPos;
    float currentDepth = length(fragToLight);
    float diskRadius = (1.0 + (currentDepth  / far_plane)) / 30.0;
     currentDepth = currentDepth- max((1.f-angle),0.07f);
     
    vec3 offset[20] = vec3[]
    (
    vec3( 1,  1,  1), vec3( 1, -1,  1), vec3(-1, -1,  1), vec3(-1,  1,  1), 
    vec3( 1,  1, -1), vec3( 1, -1, -1), vec3(-1, -1, -1), vec3(-1,  1, -1),
    vec3( 1,  1,  0), vec3( 1, -1,  0), vec3(-1, -1,  0), vec3(-1,  1,  0),
    vec3( 1,  0,  1), vec3(-1,  0,  1), vec3( 1,  0, -1), vec3(-1,  0, -1),
    vec3( 0,  1,  1), vec3( 0, -1,  1), vec3( 0, -1, -1), vec3( 0,  1, -1)
    );   
    
     float shadow=0.f,
            samples = 20;
    for(int i=0;i<samples;i++)
  if(currentDepth < (texture(shadowMap,fragToLight+offset[i]*diskRadius).r*far_plane)) 
                shadow +=1.f;
    return shadow/samples;
}

vec3 factorPointLight(PointLight light, vec3 text_color, vec3 viewDir){
    vec3 lightDir = normalize (light.position - fs_in.FragPos);
    float angle_diff = dot(lightDir,fs_in.norm);
    //отражение
    float spec = pow(max(dot(fs_in.norm,normalize(lightDir+viewDir)),0.f),material.shininess);
    //затухание
    float distance = length(light.position-fs_in.FragPos);
    float attenuation = 1.f/(distance*distance);
    //теневой фактор
    float shadow = 1.f;//shadowCalcutation(angle_diff);
    //фон
    vec3 result = light.ambient;
    //диффузная
    result+= light.diffuse*max(angle_diff,0.f)*shadow;
    //отражение
    result+= light.specular* spec*shadow;
    return result *attenuation*text_color;

}

void main()
{
   vec3 viewDir = normalize(viewPos-fs_in.FragPos);
   vec3 text_color = vec3(texture(material.diffuse1,fs_in.texCoord));
   vec3 result = vec3(0.f);
   for(int i=0;i<size_pointLight;i++)
   result += factorPointLight(pointLight[i], text_color,viewDir);
    FragColor = vec4(result,1.f);
}