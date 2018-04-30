#version 330 core
layout (location = 0) out vec4 FragColor;
layout (location = 1) out vec4 BrightColor;

in vec4 Colour;
in vec2 TexCoords;
in float eTime;

uniform sampler2D texture_diffuse1;
uniform bool Glow;

void main() 
{
	vec4 texColor = texture(texture_diffuse1, TexCoords);
	float sum  = (texColor.x + texColor.y + texColor.z);
	float alpha = exp(-sum);
	texColor.w = alpha;
    FragColor = texColor * Colour / eTime;
	
	if(Glow)
		BrightColor = FragColor * Colour / eTime;
}
