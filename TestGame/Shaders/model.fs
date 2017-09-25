#version 330 core
out vec4 FragColor;

struct DirLight {
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
 
in vec3 Position;
in vec2 TexCoords;
in vec3 Normal;  

uniform sampler2D texture_diffuse1;
uniform sampler2D texture_specular1;

uniform vec3 viewPos;
uniform DirLight dirLight;

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir);

void main()
{    
    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - Position);

    vec3 result = CalcDirLight(dirLight, norm, viewDir);

	FragColor = vec4(result, 1.0);
	//FragColor = texture(texture_diffuse1, TexCoords);
}

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(-light.direction);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 1.0);
    // combine results
    vec3 ambient = light.ambient * vec3(texture(texture_diffuse1, TexCoords));
    vec3 diffuse = light.diffuse * diff * vec3(texture(texture_diffuse1, TexCoords));
    vec3 specular = light.specular * spec * vec3(texture(texture_specular1, TexCoords));
    return (ambient + diffuse + specular);
}