
#version 330 core
in vec2 texCoord;

out vec4 FragColor;
uniform sampler2D ls_Texture;

void main()
{
   FragColor = texture2D(ls_Texture, texCoord);
}
