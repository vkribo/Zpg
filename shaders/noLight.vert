#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
uniform mat4 gModel;
uniform mat4 gProjection;
uniform mat4 gView;
void main()
{
    gl_Position = gProjection * gView * (gModel * vec4(aPos.x, aPos.y, aPos.z, 1.0));
}