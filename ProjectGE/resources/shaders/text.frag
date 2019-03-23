#version 330 core
in vec2 TexCoords;
in vec4 textColor;
in float tID;
out vec4 color;

//uniform sampler2D image;
uniform sampler2D textures[32];

void main()
{
	color = vec4(textColor.xyz, texture(textures[int(tID-1)], TexCoords).r * textColor.w);
}  