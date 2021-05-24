#version 330 core
out vec4 FragColor;
uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 ambientColor;
uniform vec3 cameraPos;
in vec3 normal;
in vec3 FragPos;
uniform vec3 lightPos;
void main()
{
    // ambient
    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * lightColor;
    // diffuse 
    vec3 norm = normalize(normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;
    // specular
    vec3 reflection = reflect(-lightDir,normal);
    vec3 cameraVec = normalize(cameraPos - FragPos);
    float specularAmount = pow(max(dot(reflection,cameraVec),0),32);
    vec3 specular = specularAmount * lightColor;
    vec3 result = (ambient + diffuse + specular) * objectColor;
    FragColor = vec4(result, 1.0);
};