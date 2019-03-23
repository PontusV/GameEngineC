#version 330 core
in vec2 TexCoords;
in vec4 spriteColor;
in float tID;
out vec4 color;

//uniform sampler2D image;
uniform sampler2D textures[32];

void main()
{
	if (tID > 0.0)
		color = texture(textures[int(tID-1)], TexCoords);
	else
		color = spriteColor;
	//color = spriteColor * texture(image, TexCoords);
}  