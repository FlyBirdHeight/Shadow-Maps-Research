#version 330 core
out vec4 FragColor;

uniform sampler2D diffuseTexture;
uniform sampler2D shadowMap;

uniform vec3 lightPos;
uniform vec3 viewPos;

in vec3 Normal;
in vec2 TexCoords;
in vec3 FragPos;
in vec4 FragLightPos;

float ShadowCalculation(vec4 FragPosLight){
    vec3 projCoords = (FragPosLight.xyz) / FragPosLight.w;
    projCoords = projCoords * 0.5 + 0.5;
    float closestDepth = texture(shadowMap, projCoords.xy).r;
    vec3 normal = normalize(Normal);
    float currentDepth = projCoords.z;
    vec3 lightDir = normalize(lightPos - FragPos);
    //计算偏移值
    float bias = max(0.05 * (1.0 - dot(normal, lightDir)), 0.005);
    float shadow = 0.0;
    //抗锯齿pcf
    vec2 texelSize = 1.0 / textureSize(shadowMap, 0);
    for(int x = -1; x < 2; ++x){
        for(int y = -1; y < 2; ++y){
            float pcfDepth = texture(shadowMap, projCoords.xy + vec2(x, y) * texelSize).r;
            shadow += currentDepth - bias > pcfDepth  ? 1.0 : 0.0;
        }
    }
    shadow /= 9.0;
    if(projCoords.z > 1.0){
        shadow = 0.0;
    }
    
    return shadow;
}

void main(){
    vec3 color = texture(diffuseTexture, TexCoords).rgb;
    vec3 normal = normalize(Normal);
    //平行光颜色
    vec3 lightColor = vec3(0.3, 0.3, 0.3);
    //环境光颜色
    vec3 ambient = 0.3 * color;
    //反射光
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(lightDir, normal), 0.0);
    vec3 diffuse = lightColor * diff;
    //聚光
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 halfwayDir = normalize(lightDir + viewDir);
    float spec = 0.0;
    spec = pow(max(dot(normal, halfwayDir), 0.0), 64.0);
    vec3 specular = spec * lightColor;
    float shadow = ShadowCalculation(FragLightPos);
    vec3 lighting = (ambient + (1-shadow) * (diffuse + specular)) * color;
    
    FragColor = vec4(lighting, 1.0);
}
