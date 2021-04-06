#version 330 core
#define NUM_SAMPLES 20
#define BLOCKER_SEARCH_NUM_SAMPLES NUM_SAMPLES
#define PCF_NUM_SAMPLES NUM_SAMPLES
#define NUM_RINGS 10
#define WIDTH_LIGHT 3
#define BIAS 1e-2

#define EPS 1e-3
#define PI 3.141592653589793
#define PI2 6.283185307179586

out vec4 FragColor;

in VS_OUT {
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoords;
    vec4 FragLightPos;
} fs_in;

uniform sampler2D texture_diffuse1;
uniform sampler2D depthMap;
uniform vec3 ks;
uniform vec3 lightPos;
uniform vec3 viewPos;
uniform vec3 lightIntensity;

highp float rand_1to1(highp float x ) {
  // -1 -1
  return fract(sin(x)*10000.0);
}

highp float rand_2to1(vec2 uv ) {
  // 0 - 1
    const highp float a = 12.9898, b = 78.233, c = 43758.5453;
    highp float dt = dot( uv.xy, vec2( a,b ) ), sn = mod( dt, PI );
    return fract(sin(sn) * c);
}

float unpack(vec4 rgbaDepth) {
    const vec4 bitShift = vec4(1.0, 1.0/256.0, 1.0/(256.0*256.0), 1.0/(256.0*256.0*256.0));
    return dot(rgbaDepth, bitShift);
}

vec2 poissonDisk[NUM_SAMPLES];

void poissonDiskSamples( const in vec2 randomSeed ) {

  float ANGLE_STEP = PI2 * float( NUM_RINGS ) / float( NUM_SAMPLES );
  float INV_NUM_SAMPLES = 1.0 / float( NUM_SAMPLES );

  float angle = rand_2to1( randomSeed ) * PI2;
  float radius = INV_NUM_SAMPLES;
  float radiusStep = radius;

  for( int i = 0; i < NUM_SAMPLES; i ++ ) {
    poissonDisk[i] = vec2( cos( angle ), sin( angle ) ) * pow( radius, 0.75 );
    radius += radiusStep;
    angle += ANGLE_STEP;
  }
}

void uniformDiskSamples( const in vec2 randomSeed ) {

  float randNum = rand_2to1(randomSeed);
  float sampleX = rand_1to1( randNum ) ;
  float sampleY = rand_1to1( sampleX ) ;

  float angle = sampleX * PI2;
  float radius = sqrt(sampleY);

  for( int i = 0; i < NUM_SAMPLES; i ++ ) {
    poissonDisk[i] = vec2( radius * cos(angle) , radius * sin(angle)  );

    sampleX = rand_1to1( sampleY ) ;
    sampleY = rand_1to1( sampleX ) ;

    angle = sampleX * PI2;
    radius = sqrt(sampleY);
  }
}

float findBlocker(vec2 uv, float depth) {
    float filterSize = 10.0 / 1024.0;
    int numBlocker = 0;
    float shadowDepth = 0.0;
    poissonDiskSamples(uv);
    float value = 0.0;
    for(int i = 0; i < BLOCKER_SEARCH_NUM_SAMPLES; ++i){
        shadowDepth = unpack(texture(depthMap, uv + poissonDisk[i] * filterSize));
        if(depth - BIAS >= shadowDepth){
            value += shadowDepth;
            numBlocker++;
        }
    }
    if(numBlocker == 0){
        return 0.0;
    }
    
    return value / float(numBlocker);
}

float PCF(float filterSize, float currentDepth, vec2 uv) {
    float shadowDepth;
    poissonDiskSamples(uv);
    float value = 0.0;
    for(int i = 0; i < PCF_NUM_SAMPLES; ++i){
        shadowDepth = unpack(texture(depthMap, uv + poissonDisk[i] * filterSize));
        if(currentDepth - BIAS <= shadowDepth){
            value += 1.0;
        }
    }
    
    return value / float(PCF_NUM_SAMPLES);
}

float PCSS(){
    vec3 coords = (fs_in.FragLightPos.xyz) / fs_in.FragLightPos.w;
    coords = coords * 0.5 + 0.5;
    // STEP 1: avgblocker depth
    float avgBlockerDepth = findBlocker(coords.xy, coords.z);
    // STEP 2: penumbra size
    if(avgBlockerDepth == 0.0){
        return 1.0;
    }
    float filterSize = ((coords.z / avgBlockerDepth - 1.0) * float(WIDTH_LIGHT)) * 10.0 / 1024.0;
    // STEP 3: filtering
    
    return PCF(filterSize, coords.z, coords.xy);
}


float useShadowMap(){
    vec3 projCoords = (fs_in.FragLightPos.xyz) / fs_in.FragLightPos.w;
    projCoords = projCoords * 0.5 + 0.5;
    float closestDepth = texture(depthMap, projCoords.xy).r;
    vec3 normal = normalize(fs_in.Normal);
    float currentDepth = projCoords.z;
    vec3 lightDir = normalize(lightPos - fs_in.FragPos);
    //计算偏移值
    float bias = max(0.05 * (1.0 - dot(normal, lightDir)), 0.005);
    
    return currentDepth - bias > closestDepth ? 1.0 :  0.0;
}

vec3 blinnPhong() {
    vec3 color = texture(texture_diffuse1, fs_in.TexCoords).rgb;
    color = pow(color, vec3(2.2));

    vec3 ambient = 0.05 * color;

    vec3 lightDir = normalize(lightPos);
    vec3 normal = normalize(fs_in.Normal);
    float diff = max(dot(lightDir, normal), 0.0);
    //  vec3 light_atten_coff =
    //    lightIntensity / pow(length(lightPos - fs_in.FragPos), 2.0);
    vec3 diffuse = diff * color;

    vec3 viewDir = normalize(viewPos - fs_in.FragPos);
    vec3 halfDir = normalize((lightDir + viewDir));
    float spec = pow(max(dot(halfDir, normal), 0.0), 32.0);
    vec3 specular = color * spec;

    float shadow = PCSS();
    vec3 radiance = (ambient + shadow * (diffuse + specular));
    vec3 phongColor = pow(radiance, vec3(1.0 / 2.2));

    return phongColor;
}

void main(void) {

    float visibility;
//    visibility = useShadowMap();
//    visibility = PCF(uShadowMap, vec4(shadowCoord, 1.0));
//    visibility = PCSS();

    vec3 phongColor = blinnPhong();

//    FragColor = vec4(vec3(visibility, visibility, visibility), 1.0);
    FragColor = vec4(phongColor, 1.0);
}
