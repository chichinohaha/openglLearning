#version 330 core
out vec4 FragColor;

in vec2 TexCoords;
uniform bool invert;
uniform sampler2D sceneTexture;
void main()
{  
	if(invert){
		FragColor = vec4(vec3(1.0 - texture(sceneTexture, TexCoords)), 1.0);
	}
	else {
		FragColor = texture(sceneTexture, TexCoords);
	}
}