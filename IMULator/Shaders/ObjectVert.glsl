
#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNorm;

out vec3 Normal;
out vec3 FragPos;

uniform mat4 ob_Matrix;
uniform mat4 Model;

void main()
{
    FragPos = vec3(Model  * vec4(aPos, 1.0));
    Normal = mat3(transpose(inverse(Model))) * aNorm;
    gl_Position = ob_Matrix * vec4(aPos, 1.0);
}
