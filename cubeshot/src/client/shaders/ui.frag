#version 330 core

out vec4 FragColor;

uniform vec3 color;

void main()
{
	FragColor = vec3(color, 1.0);
}