#version 460 core

out vec4 FragColor;

in vec3 Normal;
in vec2 TexCoords;
in vec3 FragPos;

void main()
{
    vec3 color = normalize(Normal) * 0.5 + 0.5;
    
    float ambientStrength = 0.6;
    vec3 ambient = ambientStrength * vec3(1.0, 1.0, 1.0); // 백색광
    
    FragColor = vec4(color * ambient, 1.0);
}