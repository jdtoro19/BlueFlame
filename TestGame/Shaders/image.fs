#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D texture;
uniform vec3 colour;
uniform float alpha;

void main()
{             
    FragColor = vec4(texture(texture, TexCoords).rgb + colour, texture(texture, TexCoords).a * alpha);
}