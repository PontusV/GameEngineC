#version 330 core
layout (location = 0) in vec2 position;
layout (location = 1) in vec4 color;

out vec4 spriteColor;

uniform mat4 model = mat4(1.0);
uniform mat4 projection;


void main()
{
	spriteColor = color;
    gl_Position = projection * model * vec4(position.xy, 0.0, 1.0);
}