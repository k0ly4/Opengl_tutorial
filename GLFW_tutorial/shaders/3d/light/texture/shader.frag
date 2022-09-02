#version 330 core
out vec4 FragColor;

in vec3 FragPos;
in vec3 norm;
in vec2 tex_coord;

uniform  sampler2D diffuse1;
uniform float ambientFactor;
uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 viewPos;
uniform float specularMaterial;


void main()
{          
    vec3 color = texture(diffuse1,tex_coord).rgb; 
    // Фоновая составляющая
    vec3 ambient = ambientFactor * color;
	
    // Диффузная составляющая
    vec3 lightDir = normalize(lightPos - FragPos);
    
    float diff = max(dot(lightDir, norm), 0.0);
    vec3 diffuse = diff * color;//*lightColor;
	
    // Отраженная составляющая
    vec3 viewDir = normalize(viewPos - FragPos);
 
    float specular = pow(max(dot(norm, normalize(lightDir + viewDir)), 0.0), 16.0);
    FragColor = vec4(ambient + diffuse + vec3(specularMaterial)*specular , 1.0);
}