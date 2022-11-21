#version 330 core
struct Light {
   vec3 position;
   vec3 direction;
   vec3 ambient;
   vec3 diffuse;
   vec3 specular;
   float cutOff;
   float outerCutOff;
   float constant;
   float linear;
   float quadratic;
};

vec3 CalcDirLight(Light light, vec3 normal, vec3 viewDir)
{
   //vec3 objColor = vec3(0.85f, 0.85f, 0.85f);
   vec3 lightDir = normalize(-light.direction);
   // diffuse shading
   float diff = max(dot(normal, lightDir), 0.0);
   // specular shading
   vec3 reflectDir = reflect(-lightDir, normal);
   float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
   // combine results
   vec3 ambient  = light.ambient;//  * objColor;
   vec3 diffuse  = light.diffuse  * diff;// * objColor;
   vec3 specular = (dot(normal, lightDir) < 0.0)? vec3(0.0) : light.specular * spec; //* objColor;
   return (ambient + diffuse + specular);
}

vec3 CalcPointLight(Light light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
   //vec3 objColor = vec3(0.85f, 0.85f, 0.85f);
   vec3 lightDir = normalize(light.position - fragPos);
   // diffuse shading
   float diff = max(dot(normal, lightDir), 0.0);
   // specular shading
   vec3 reflectDir = reflect(-lightDir, normal);
   float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
   // attenuation
   float distance    = length(light.position - fragPos);
   float attenuation = 1.0 / (light.constant + light.linear * distance +
   light.quadratic * (distance * distance));
   // combine results
   vec3 ambient  = light.ambient;//  * objColor;
   vec3 diffuse  = light.diffuse  * diff;// * objColor;
   vec3 specular = (dot(normal, lightDir) < 0.0)? vec3(0.0) : light.specular * spec;// * objColor;
   ambient  *= attenuation;
   diffuse  *= attenuation;
   specular *= attenuation;
   return (ambient + diffuse + specular);
}

vec3 CalcSpotLight(vec3 diffuse, vec3 specular, vec3 worldPosition, vec3 normalVector, Light light, float shininess, vec3 viewPos) {
   vec3 lightVector = normalize(light.position - worldPosition);
   float theta = dot(lightVector, normalize(-light.direction));
   float epsilon = light.cutOff - light.outerCutOff;
   float intensity = smoothstep(0.0, 1.0, (theta - light.outerCutOff) / epsilon);

   if(theta > light.outerCutOff) {
      float dot_product = max(dot(lightVector, normalVector), 0.0);
      diffuse += (dot_product * light.diffuse);

      float spec = pow(max(dot(reflect(lightVector, normalVector), normalize(worldPosition - viewPos)), 0.0), shininess);
      specular += (light.specular * spec);

      diffuse *= intensity;
      specular *= intensity;
   }

   return diffuse + specular;
}

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;
uniform vec3 viewPos;
#define MAX_DIR_LIGHTS 100
uniform int len_dir_lights;
uniform Light dir_lights[MAX_DIR_LIGHTS];
#define MAX_POINT_LIGHTS 100
uniform int len_point_lights;
uniform Light point_lights[MAX_POINT_LIGHTS];
#define MAX_SPOT_LIGHTS 100
uniform int len_spot_lights;
uniform Light spot_lights[MAX_SPOT_LIGHTS];
uniform Light flashlight;
uniform sampler2D textureUnitID;
out vec4 FragColor;
void main()
{
   vec3 norm = normalize(Normal);
   vec3 viewDir = normalize(viewPos - FragPos);
   vec3 result = vec3(0.0);
   for (int i = 0; i < len_dir_lights; i++)
           result += CalcDirLight(dir_lights[i], norm, viewDir);
   for (int i = 0; i < len_point_lights; i++)
           result += CalcPointLight(point_lights[i], norm, FragPos, viewDir);
   for (int i = 0; i < len_spot_lights; i++)
           result += CalcSpotLight(vec3(0.0), vec3(0.0), FragPos, norm, spot_lights[i], 32, viewPos);
   result += CalcSpotLight(vec3(0.0), vec3(0.0), FragPos, norm, flashlight, 32, viewPos);
   FragColor = vec4(result, 1.0) * texture(textureUnitID, TexCoords);
}