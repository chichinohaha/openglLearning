#version 330 core
out vec4 FragColor;
uniform vec3 objectColor;
uniform vec3 lightColor;
in vec3 normal;
uniform vec3 lightPos;
void main()
{
	FragColor = vec4(lightColor * objectColor,1.0f );
};