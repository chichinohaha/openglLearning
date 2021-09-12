#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
//layout (location = 2) in vec3 aOffset;

uniform int selectedID;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 model;
out vec3 fColor;
//layout (std140) uniform Matrices
//{
//    mat4 projection;
//    mat4 view;
//};
void main()
{
    if(gl_InstanceID == selectedID){
        fColor = vec3(1,1,1);
    } else {
        fColor = aColor;
    }
    gl_Position = projection * view * model * vec4(aPos, 1.0);
}