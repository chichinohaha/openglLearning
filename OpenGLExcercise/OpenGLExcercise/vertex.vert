#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;
out vec4 vertexColor;
out vec3 normal;
out vec3 FragPos;
out vec2 TexCoords;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 normalTransform;
void main()
{
    // 注意乘法要从右向左读
    gl_Position = projection * view * model * vec4(aPos, 1.0);
    normal =  vec3(normalTransform * vec4(aNormal,0));
    FragPos = aPos;
    TexCoords = aTexCoords;
}