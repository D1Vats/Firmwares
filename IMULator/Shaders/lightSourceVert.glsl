
#version 330 core
layout (location = 0) in vec3 aPosition;
layout (location = 1) in vec2 aTexture;

out vec2 texCoord;

uniform mat4 ls_Matrix;

void main()
{
    gl_Position = ls_Matrix * vec4(aPosition, 1.0);
    texCoord = aTexture;
}
