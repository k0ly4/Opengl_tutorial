#version 330 core

out vec4 FragColor;

in VS_OUT{
    vec2 texCoord;
    vec3 FragPos;
    vec3 lightDir;
    vec3 viewDir;
     vec3 lightPos;
     vec3 fragToLight;
}fs_in;
////материл
struct Material{
    sampler2D 
    diffuse,
    specular,
    normal,
    depth;

    float shininess;
};
//источник света
struct PointLight{
    vec3 
        factor,
        ambient,
        diffuse,
        specular;
};

uniform PointLight pointLight;
uniform Material material;
uniform float far_plane;
uniform samplerCube shadowMap;
uniform float height_scale;

float shadowCalcutation(float angle){
    float currentDepth = length(fs_in.fragToLight);
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
  if(currentDepth < (texture(shadowMap,fs_in.fragToLight+offset[i]*diskRadius).r*far_plane)) 
                shadow +=1.f;
    return shadow/samples;
}

vec3 factorPointLight(PointLight light, vec3 text_color, vec3 viewDir,vec3  norm){
   
    float angle_diff = dot(norm,fs_in.lightDir);
    //отражение
    float spec = pow(max(dot(norm,normalize(fs_in.lightDir+viewDir)),0.f),material.shininess);
    //затухание
    float distance = length(fs_in.lightPos-fs_in.FragPos);
    float attenuation = 1.f/ (light.factor.x + light.factor.y*distance+ light.factor.z*distance*distance);
    //теневой фактор
   float shadow = shadowCalcutation(angle_diff);
    //фон
    vec3 result = light.ambient*text_color;
    //диффузная
    result+= light.diffuse*max(angle_diff,0.f)*text_color*shadow;
    //отражение
    result+= light.specular* spec*shadow;
    return result *attenuation;

}
vec2 ParallaxMapping(vec2 texCoords,vec3 viewDir){

    const float minLayers = 8.f;
    const float maxLayers = 64.f;
    float numLayers = mix(maxLayers,minLayers,max(dot(vec3(0.f,0.f,1.f),viewDir),0.f));
    float layerDepth = 1.f/numLayers;
    float currentLayerDepth = 0.f;
    vec2 P = viewDir.xy*height_scale;
    vec2 deltaTexCoords = P/numLayers;
    vec2 currentTexCoords = texCoords;
    float currentDepthMapValue = texture(material.depth,currentTexCoords).r;
    while(currentLayerDepth<currentDepthMapValue){
    currentTexCoords -= deltaTexCoords;
    currentDepthMapValue = texture(material.depth,currentTexCoords).r;
    currentLayerDepth += layerDepth;
    }
    vec2 prevTexCoords = currentTexCoords +deltaTexCoords;

    float afterDepth = currentDepthMapValue - currentLayerDepth;
    float beforeDepth = texture(material.depth,prevTexCoords).r - currentLayerDepth + layerDepth;

    float weight = afterDepth/(afterDepth-beforeDepth);
    vec2 finalTexCoords = prevTexCoords*weight + currentTexCoords*(1.f-weight);
    return finalTexCoords;
}
void main()
{   
    vec2 texCoord = 
    ParallaxMapping(fs_in.texCoord,fs_in.viewDir);
    if(texCoord.x >1.f || texCoord.y>1.f ||texCoord.x<0.f||texCoord.y<0.f) discard;
   vec3 text_color = vec3(texture(material.diffuse,texCoord));
   vec3 norm = texture(material.normal,texCoord).rgb;
    norm =normalize(norm*2.f-1.f);

   vec3 result = vec3(0.f);
   result += factorPointLight(pointLight, text_color,fs_in.viewDir,norm);
    FragColor = vec4(result,1.f);
}