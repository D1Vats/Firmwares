
#version 330 core
out vec4 FragColor;

uniform vec3 lightColor;
uniform vec3 objectColor;

in vec3 Normal;
in vec3 FragPos;

uniform vec3 lightPos;

void main()
{
    //Ambient
    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * lightColor;
    // Diffuse
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(FragPos - lightPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

    vec3 result = (ambient + diffuse) * objectColor;
   FragColor = vec4(result, 1.0);
}
