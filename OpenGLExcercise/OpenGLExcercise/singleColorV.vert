#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;
out vec4 vertexColor;
out vec3 normal;
out vec3 FragPos;
out vec2 TexCoords;
uniform mat4 model;
uniform vec3 scale;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 normalTransform;
void main()
{
    // 注意乘法要从右向左读
    normal =  vec3(normalTransform * vec4(aNormal,0));
    gl_Position = projection * view * model * vec4(aPos + normalize(normal) * scale, 1.0);    
    FragPos = aPos;
    TexCoords = aTexCoords;
}