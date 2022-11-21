#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;
uniform mat4 gModel;
uniform mat4 gProjection;
uniform mat4 gView;
out vec3 FragPos;
out vec3 Normal;
out vec2 TexCoords;
void main()
{
   TexCoords = aTexCoords;
   gl_Position = gProjection * gView * (gModel * vec4(aPos.x, aPos.y, aPos.z, 1.0));
   FragPos = vec3(gModel * vec4(aPos, 1.0));
   Normal = mat3(transpose(inverse(gModel))) * aNormal;
}