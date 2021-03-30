#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec3 aColor;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

out VS_OUT {
    vec3 FragPos;
    vec3 Normal;
    vec3 Color;
} vs_out;

void main(){
    vs_out.FragPos = vec3(model * vec4(aPos, 1.0));
    vs_out.Normal = transpose(inverse(mat3(model))) * aNormal;
    vs_out.Color = aColor;
    gl_Position = projection * view * model * vec4(aPos, 1.0);
}
