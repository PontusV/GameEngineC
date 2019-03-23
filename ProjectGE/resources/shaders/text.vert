#version 330 core
layout (location = 0) in vec2 position;
layout (location = 1) in vec4 color;
layout (location = 2) in vec2 texture;
layout (location = 3) in float textureID;

out vec2 TexCoords;
out vec4 textColor;
out float tID;

uniform mat4 model = mat4(1.0);
uniform mat4 projection;


void main()
{
	tID = textureID;
	TexCoords = texture;
	textColor = color;
	gl_Position = projection * model * vec4(position.xy, 0.0, 1.0);
}