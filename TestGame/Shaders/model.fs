#version 330 core
layout (location = 0) out vec4 FragColor;
layout (location = 1) out vec4 BrightColor;

struct DirLight {
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct PointLight {
    float constant;
    float linear;
    float quadratic;

    vec3 position;
	
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
 
in vec3 Position;
in vec2 TexCoords;
in vec3 Normal;
in vec3 Colour;
in vec4 weight;
in vec4 id;

#define numDirLights 10
#define numPointLights 10

uniform sampler2D texture_diffuse1;
uniform sampler2D texture_specular1;

uniform vec3 viewPos;
uniform DirLight dirLight[numDirLights];
uniform PointLight pointLight[numPointLights];

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir);
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);

void main()
{    
    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - Position);
    vec3 result = vec3(0,0,0);

    for(int i = 0; i < numDirLights; i++) {
		result += CalcDirLight(dirLight[i], norm, viewDir);
	}

    for(int i = 0; i < numPointLights; i++) {
		result += CalcPointLight(pointLight[i], norm, Position, viewDir);   
	}    
	
	// check whether result is higher than some threshold, if so, output as bloom threshold color
    float brightness = dot(result, vec3(0.2126, 0.7152, 0.0722));
    if(brightness > 1.0)
        BrightColor = vec4(result + Colour, 1.0);
    else
        BrightColor = vec4(0.0, 0.0, 0.0, 1.0);
		
    FragColor = vec4(result + Colour, 1.0);
	
	//Display only colours
	//FragColor = vec4(Colour, 1.0);
    
	// Display bone weights
	//vec4 weightsColor = vec4(weight.xyz,1.0);
	//FragColor = weightsColor;
	
	// Display only textures
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

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 1.0);//material.shininess);
    // attenuation
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    
    // combine results
    vec3 ambient = light.ambient * vec3(texture(texture_diffuse1, TexCoords));
    vec3 diffuse = light.diffuse * diff * vec3(texture(texture_diffuse1, TexCoords));
    vec3 specular = light.specular * spec * vec3(texture(texture_specular1, TexCoords));
	// If light is 0 do not add to other lights
	if(ambient != vec3(0,0,0)) {
		ambient *= attenuation;
		diffuse *= attenuation;
		specular *= attenuation;
	}
    return (ambient + diffuse + specular);
}