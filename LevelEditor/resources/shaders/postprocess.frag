#version 330 core
in vec2 TexCoord;
out vec4 color;

uniform bool invert;
uniform bool grayscale;
uniform sampler2D scene;

void main()
{
	if (grayscale) {
		color = texture(scene, TexCoord);
		float avg = 0.2126 * color.r + 0.7152 * color.g + 0.0722 * color.b;
		color = vec4(avg, avg, avg, 1.0);
	} else {
		color = texture(scene, TexCoord);
	}
	
	if (invert) {
		color = vec4(1.0-color.r, 1.0-color.g, 1.0-color.b, 1.0);
	}
}