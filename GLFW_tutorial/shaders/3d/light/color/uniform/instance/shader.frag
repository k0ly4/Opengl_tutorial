#version 330 core
out vec4 FragColor;

in vec3 FragPos;
in vec3 norm;
uniform vec3 color;

uniform float ambientFactor;
uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 viewPos;
uniform float specularMaterial;

void main()
{           
    // Фоновая составляющая
    vec3 ambient = ambientFactor * color;
	
    // Диффузная составляющая
    vec3 lightDir = normalize(lightPos - FragPos);
    
    float diff = max(dot(lightDir, norm), 0.0);
    vec3 diffuse = diff * color;//*lightColor;
	
    // Отраженная составляющая
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
 
    float specular = pow(max(dot(norm, normalize(lightDir + viewDir)), 0.0), 16.0);
    FragColor = vec4(ambient + diffuse + vec3(specularMaterial)*specular , 1.0);
}