#version 330 core
#define LENGTH_OF_POINTLIGHT 4
#define LENGTH_OF_SPOTLIGHT 4

struct Material{
    sampler2D diffuse1;
    sampler2D specular1;
    float shininess;
};
struct DirectionLight {
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct PointLight {
    vec3 position;
    
    float constant;
    float linear;
    float quadratic;
	
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct SpotLight {
    vec3 position;
    vec3 direction;
    float cutOff;
    float outerCutOff;
  
    float constant;
    float linear;
    float quadratic;
  
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;       
};
in vec3 normal;
in vec3 FragPos;
in vec2 TexCoords;
out vec4 FragColor;
uniform Material material;
uniform DirectionLight directionLight;
uniform PointLight pointLights[LENGTH_OF_POINTLIGHT];
uniform SpotLight spotLight;
uniform vec3 objectColor;
uniform vec3 cameraPos;
vec3 getDirectionLight(DirectionLight light,vec3 uNormal,vec3 dirToCamera){
    // ambient
    vec3 ambientColor = light.ambient * texture2D(material.diffuse1,TexCoords).rgb;
    // diffuse
    float diffuseIntensity = max( dot(-light.direction,uNormal),0);
    vec3 diffuseColor = diffuseIntensity * light.diffuse * texture2D(material.diffuse1,TexCoords).rgb;
    // specular pow(max(dot(R,cam),0),material.shininess)
    vec3 R = normalize(reflect(light.direction,uNormal));
    float specularIntensity= pow(max(dot(R,dirToCamera),0),material.shininess);
    vec3 specularColor = specularIntensity * light.specular * texture2D(material.specular1,TexCoords).rgb;
    vec3 result = diffuseColor + specularColor + ambientColor;
    return result;
}

vec3 getPointLight(PointLight light, vec3 uNormal, vec3 dirToCamera){
    // attenuation
    float dist = length(light.position - FragPos);
    float attenuation = 1/(light.constant + light.linear * dist + light.quadratic * pow(dist,2));
    // ambient
    vec3 ambientColor = light.ambient * texture2D(material.diffuse1,TexCoords).rgb;
    // diffuse
    float diffuseIntensity = max( dot(normalize(light.position - FragPos),uNormal),0);
    vec3 diffuseColor = diffuseIntensity * light.diffuse * attenuation * texture2D(material.diffuse1,TexCoords).rgb;
    // specular
    vec3 R = normalize(reflect(normalize(light.position - FragPos),uNormal));
    float specularIntensity= pow(max(dot(R,dirToCamera),0),material.shininess);
    vec3 specularColor = specularIntensity * light.specular * attenuation * texture2D(material.specular1,TexCoords).rgb;
    vec3 result = ambientColor + diffuseColor + specularColor;
    return result;
}

vec3 getSpotLight(SpotLight light, vec3 uNormal,vec3 dirToCamera){
    vec3 lightDir = normalize(FragPos - light.position );
    // spot light intensity (soft edges) 由相似三角形可以推出公式
    float theta = dot(lightDir,normalize(light.direction));
    float epsilon = (light.cutOff - light.outerCutOff);
    float intensity = clamp((theta - light.outerCutOff)/epsilon,0.0,1.0);
    // attenuation
    float dist = length(light.position - FragPos);
    float attenuation = 1/(light.constant + light.linear * dist + light.quadratic * pow(dist,2));
    // ambient
    vec3 ambientColor = light.ambient * texture2D(material.diffuse1,TexCoords).rgb * attenuation;
    // diffuse
    float diffuseIntensity = max( dot(normalize(light.position - FragPos),uNormal),0) ;
    vec3 diffuseColor = diffuseIntensity * light.diffuse * attenuation * texture2D(material.diffuse1,TexCoords).rgb * intensity;    
    // specular
    vec3 R = reflect(lightDir,uNormal);
    float specularIntensity = pow(max(dot(R,dirToCamera),0),material.shininess);
    vec3 specularColor = specularIntensity *light.specular * texture2D(material.specular1,TexCoords).rgb * intensity; 
    vec3 result = ambientColor + diffuseColor + specularColor;
    return result;
}
void main()
{
    vec3 uNormal = normalize(normal);
    vec3 dirToCamera = normalize(cameraPos - FragPos);
    vec3 finalResult = vec3(0,0,0);
    finalResult += getDirectionLight(directionLight,uNormal,dirToCamera);
    for(int i = 0;i<LENGTH_OF_POINTLIGHT;i++){
        finalResult += getPointLight(pointLights[i],uNormal,dirToCamera);
    }
    finalResult += getSpotLight(spotLight,uNormal,dirToCamera);
    FragColor = vec4(finalResult,1.0);
};