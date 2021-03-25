#version 330 core
in vec4 FragPos;


uniform vec3 lightPos;
uniform float far_plane;

void main(){
    float lightDepth = length(FragPos.xyz - lightPos);
    
    lightDepth = lightDepth / far_plane;
    
    gl_FragDepth = lightDepth;
}
