#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aLightColor;

uniform mat4 projection;
uniform mat4 model;
uniform mat4 view;

out VS_OUT{
    vec3 FragPos;
    vec3 LightColor;
}


