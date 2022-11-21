#version 330 core

layout (location = 0) in vec3 aPos;
out vec3 TexCoords;
uniform mat4 gProjection;
uniform mat4 gView;
void main()
{
    TexCoords = aPos;
    mat4 view = mat4(mat3(gView));
    vec4 pos = gProjection * view * vec4(aPos, 1.0);
    gl_Position = pos.xyww;
}