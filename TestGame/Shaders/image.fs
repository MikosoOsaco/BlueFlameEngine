#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D texture1;
uniform vec3 colour;
uniform float alpha;

void main()
{             
    FragColor = vec4(texture(texture1, TexCoords).rgb + colour, texture(texture1, TexCoords).a * alpha);
}