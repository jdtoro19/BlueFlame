#version 330 core
layout (location = 0) in vec3 sPosition;
layout (location = 1) in vec3 sNormal;
layout (location = 2) in vec2 sTexCoords;
layout (location = 3) in vec3 sColour;

out vec3 Position;
out vec3 Normal;
out vec2 TexCoords;
out vec3 Colour;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    Position = vec3(model * vec4(sPosition, 1.0));
    Normal = mat3(transpose(inverse(model))) * sNormal;  
    
    gl_Position = projection * view * vec4(Position, 1.0);

    Colour = sColour;
	
	TexCoords = sTexCoords;	
}