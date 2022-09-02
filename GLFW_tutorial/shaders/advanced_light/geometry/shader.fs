#version 330 core
 layout(location =0) out vec3 gPosition;
 layout(location =1)out vec3 gNormal;
 layout(location =2) out vec4 gAlbedoSpec;


in VS_OUT{
    vec2 texCoord;
    vec3 FragPos;
   vec3  norm;
}fs_in;

struct Material{
    sampler2D 
    diffuse1,
    specular1;
};
uniform Material material;
void main()
{
           
  gPosition = fs_in.FragPos;

   gNormal = normalize(fs_in.norm);
  // gNormal = normalize(fs_in.TBN*(gNormal*2.f-1.f));

  gAlbedoSpec.rgb = texture(material.diffuse1,fs_in.texCoord).rgb;

  gAlbedoSpec.a=texture(material.specular1,fs_in.texCoord).r;
   
}