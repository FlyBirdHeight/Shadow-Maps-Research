#version 330 core
out vec4 FragColor;

in VS_OUT {
    vec3 FragPos;
    vec3 Normal;
    vec3 Color;
    vec2 TexCoords;
} fs_in;

uniform vec3 lightPos;
uniform vec3 viewPos;

uniform sampler2D planeTexture;
uniform bool is_light;

void main(){
    vec3 color = fs_in.Color;
    vec3 normal = normalize(fs_in.Normal);
    vec3 fragPos = fs_in.FragPos;
    vec3 lightDir = normalize(lightPos - fragPos);
    vec3 viewDir = normalize(viewPos - FragPos);
    
    vec3 ambient = 0.3 * color;
    
    float diff = max(dot(lightDir, normal), 0.0);
    vec3 diffuse = diff * color;
    
    vec3 halfwayDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(halfwayDir, normal), 0.0), 64.0);
    vec3 specular = spec * color;
    
    vec3 light = ambient + diffuse + specular;
    
    if(is_light){
        FragColor = vec4(light, 1.0);
    }else{
        FragColor = vec4(fs_in.Color, 1.0);
    }
}
