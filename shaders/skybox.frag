#version 330 core
out vec4 FragColor;
in vec3 TexCoords;
uniform samplerCube textureUnitID;
void main()
{
    //FragColor = texture(textureUnitID, TexCoords);
    FragColor = vec4(1.0);
}