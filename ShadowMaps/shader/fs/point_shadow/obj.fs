#version 330 core
in VS_OUT {
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoords;
} fs_in;

uniform sampler2D texture_diffuse1;
uniform samplerCube depthMap;

uniform vec3 lightPos;
uniform vec3 viewPos;

uniform float far_plane;

out vec4 FragColor;

float CalculateShadow(vec3 FragPos){
    vec3 fragLightPos = FragPos - lightPos;
    float closestDepth = texture(depthMap, fragLightPos).r;
    closestDepth *= far_plane;
    
    float currentDepth = length(fragLightPos);
    float bias = 0.05;
    float shadow = currentDepth - bias > closestDepth ? 0.0 : 1.0;
    
    return shadow;
}

void main(){
    vec3 color = texture(texture_diffuse1, fs_in.TexCoords).rgb;
    vec3 normal = normalize(fs_in.Normal);
    //平行光颜色
    vec3 lightColor = vec3(0.3, 0.3, 0.3);
    //环境光颜色
    vec3 ambient = 0.3 * color;
    //反射光
    vec3 lightDir = normalize(lightPos - fs_in.FragPos);
    float diff = max(dot(lightDir, normal), 0.0);
    vec3 diffuse = lightColor * diff;
    //聚光
    vec3 viewDir = normalize(viewPos - fs_in.FragPos);
    vec3 halfwayDir = normalize(lightDir + viewDir);
    float spec = 0.0;
    spec = pow(max(dot(normal, halfwayDir), 0.0), 64.0);
    vec3 specular = spec * lightColor;
    float shadow = CalculateShadow(fs_in.FragPos);
    vec3 lighting = (ambient + (1 - shadow) * (diffuse + specular)) * color;
    
    FragColor = vec4(lighting, 1.0);
}
