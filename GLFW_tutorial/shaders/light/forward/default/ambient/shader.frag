#version 330 core

out vec4 FragColor;

in vec3 FragPos;
in vec2 TexCoord;
//Uniform------------------------------------------------
uniform float ambientFactor;
//Material------------------------------------------------
uniform int configMaterial;
uniform vec4 baseColor;
uniform sampler2D diffuse;

void main()
{        
    vec4 materialColor = configMaterial == 1 ? texture(diffuse, TexCoord) * baseColor : baseColor;
    vec3 Albedo = materialColor.rgb;
    vec3 lighting = Albedo * ambientFactor; // фоновая составляющая
    FragColor = vec4(lighting, materialColor.a);
}





