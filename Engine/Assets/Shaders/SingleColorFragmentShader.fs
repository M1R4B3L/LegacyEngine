#version 460 core
out vec4 FragColor;

uniform vec4 selectedColor;

void main()
{
    FragColor = selectedColor;
}