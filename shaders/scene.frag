#version 330 core
in vec3 FragPos;
in vec3 Normal;
uniform vec3 viewPos;
out vec4 FragColor;
void main()
{
    vec3 lightColor = vec3(1.0f, 1.0f, 1.0f);
    float ambientStrength = 0.2f;
    vec3 ambient = ambientStrength * lightColor;
    vec3 lightPos = vec3(10.f, 10.f, 30.f);
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 objectColor = vec3(0.f, 0.3f, 0.5f);
    vec3 diffuse = diff * lightColor;
    float specularStrength = 0.5;
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = (dot(norm, lightDir) < 0.0)? vec3(0.0) : specularStrength * spec * lightColor;
    vec3 result = (ambient + diffuse + specular) * objectColor;
    FragColor = vec4(result, 1.0);
}