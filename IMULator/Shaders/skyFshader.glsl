
#version 330 core
in vec2 texCoords;

out vec4 FragColor;
uniform sampler2D sTexture;

void main()
{
   FragColor =texture2D(sTexture, texCoords);
}
