#version 330 core
layout (location = 0) in vec2 vertex;
layout (location = 1) in vec2 texcoord;
out vec2 TexCoord;

void main()
{
	gl_Position = vec4(vertex.xy, 0.0f, 1.0f);
	TexCoord = texcoord;
}  