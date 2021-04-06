#version 330 core
out vec4 FragColor;

in VS_OUT {
    vec3 FragPos;
    vec3 Normal;
    vec3 Color;
} fs_in;

uniform vec3 lightPos;
uniform vec3 viewPos;

uniform bool is_light;

void main(){
    vec3 color = fs_in.Color;
    vec3 normal = normalize(fs_in.Normal);
    
    vec3 lightColor = vec3(10.0);
    vec3 fragPos = fs_in.FragPos;
    vec3 lightDir = normalize(lightPos - fragPos);
    vec3 viewDir = normalize(viewPos - fragPos);
    
    vec3 ambient = lightColor;
    
    float diff = max(dot(lightDir, normal), 0.0);
    vec3 diffuse = diff * lightColor;
    
    vec3 halfwayDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(halfwayDir, normal), 0.0), 64.0);
    vec3 specular = spec * lightColor;
    
    vec3 light = (ambient + diffuse + specular) * color;
    
    if(is_light){
        FragColor = vec4(light, 1.0);
    }else{
        FragColor = vec4(fs_in.Color, 1.0);
    }
}
