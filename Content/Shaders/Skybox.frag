#version 330

in vec3 vDirection;

out vec4 FragColor;

uniform samplerCube uSkybox;

void main()
{
    FragColor = texture(uSkybox, normalize(vDirection));
}