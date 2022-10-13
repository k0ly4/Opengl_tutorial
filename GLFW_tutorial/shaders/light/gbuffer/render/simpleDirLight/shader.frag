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

//DirectionLight----------------------------------------------------------
struct DirLight{

    vec3 direction;
    vec3 color;
    sampler2DShadow map;

};
uniform DirLight d_light;

//Calculate float factor of cascade shadow for direction light
//angle =dot(normal, lightDir)
float factorShadow(vec4 FragPos,float angle){
   
    vec4 fragPosLightSpace = gWVP * FragPos;
    // perform perspective divide
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    // transform to [0,1] range
    projCoords = projCoords * 0.5 + 0.5;
    // get depth of current fragment from light's perspective
    float closestDepth = textureProj(d_light.map, fragPosLightSpace);   
    //float currentDepth = projCoords.z; 

   // float shadow = currentDepth > closestDepth  ? 1.0 : 0.0;  
    return closestDepth;
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
        float shadow =factorShadow(vec4(FragPos,1.f), dotNormal_Dir);                    
        
        lighting +=  (shadow)*(diffuse + vec3(Specular*spec)); //debug

    FragColor = vec4(lighting, 1.0);
}





