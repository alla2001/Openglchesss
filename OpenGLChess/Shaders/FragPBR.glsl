#version 330 core
out vec4 fragColor;
in vec3 normal;
in vec3 fragPos;
in vec3 viewDir;
in vec2 TexCoords;

struct Light{
 float intensity;
 float ambient;
 vec3 color;
 vec3 pos;
};


struct Material {
sampler2D albedoMap;
sampler2D normalMap;
sampler2D metallicMap;
sampler2D roughnessMap;
sampler2D aoMap;
sampler2D cubeMap;
}; 
uniform samplerCube shadowMap;
struct Debug{
bool geometry;
bool fresnel;
bool distribution;
}; 
uniform vec3 camPos;
uniform Debug debug;
uniform Material material;

uniform int numberOfLights;
uniform Light lights[4];
const float PI = 3.14159265359;
vec3 sampleOffsetDirections[20] = vec3[]
(
   vec3( 1,  1,  1), vec3( 1, -1,  1), vec3(-1, -1,  1), vec3(-1,  1,  1), 
   vec3( 1,  1, -1), vec3( 1, -1, -1), vec3(-1, -1, -1), vec3(-1,  1, -1),
   vec3( 1,  1,  0), vec3( 1, -1,  0), vec3(-1, -1,  0), vec3(-1,  1,  0),
   vec3( 1,  0,  1), vec3(-1,  0,  1), vec3( 1,  0, -1), vec3(-1,  0, -1),
   vec3( 0,  1,  1), vec3( 0, -1,  1), vec3( 0, -1, -1), vec3( 0,  1, -1)
);   
float ShadowCalculation(vec3 fragPos,vec3 lightPos)
{

    vec3 fragToLight = fragPos - lightPos;
    float currentDepth = length(fragToLight);

    float result = 0;
    int samples  = 20;
    float bias = 0.05; 
    float diskRadius = 0.05;
    for(int i = 0; i < samples; ++i)
    {
        float closestDepth = texture(shadowMap, fragToLight + sampleOffsetDirections[i] * diskRadius).r;
        closestDepth *= 100;   // undo mapping [0;1]
        if(currentDepth - bias > closestDepth)
            result += 1.0;
    }


    float shadow =result/samples;

    return shadow;
}  
vec2 NormalToEquirectangularUv(vec3 normal)
{
    float phi = acos(-normal.y);
    float theta = atan(normal.z, normal.x) + PI;
    vec2 uv = vec2(theta / (2*PI), -phi / PI);
    return uv;
}
float DistributionGGX(vec3 N, vec3 H, float roughness)
{
    float a      = roughness*roughness;
    float a2     = a*a;
    float NdotH  = max(dot(N, H), 0.0);
    float NdotH2 = NdotH*NdotH;
	
    float num   = a2;
    float denom = (NdotH2 * (a2 - 1.0) + 1.0);
    denom = PI * denom * denom;
	
    return num / denom;
}

float GeometrySchlickGGX(float NdotV, float roughness)
{
    float r = (roughness + 1.0);
    float k = (r*r) / 8.0;

    float num   = NdotV;
    float denom = NdotV * (1.0 - k) + k;
	
    return num / denom;
}
float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness)
{
    float NdotV = max(dot(N, V), 0.0);
    float NdotL = max(dot(N, L), 0.0);
    float ggx2  = GeometrySchlickGGX(NdotV, roughness);
    float ggx1  = GeometrySchlickGGX(NdotL, roughness);
	
    return ggx1 * ggx2;
}
vec3 fresnelSchlick(float cosTheta, vec3 F0)
{
    return F0 + (1.0 - F0) * pow(clamp(1.0 - cosTheta, 0.0, 1.0), 5.0);
}  
vec3 fresnelSchlickRoughness(float cosTheta, vec3 F0, float roughness)
{
    return F0 + (max(vec3(1.0 - roughness), F0) - F0) * pow(clamp(1.0 - cosTheta, 0.0, 1.0), 5.0);
}   
void main()
{
   vec3 albedo     = texture(material.albedoMap, TexCoords).rgb;
   vec3 normalM     = texture(material.normalMap, TexCoords).rgb;
   float metallic  = texture(material.metallicMap, TexCoords).r;
   float roughness = texture(material.roughnessMap, TexCoords).r;
   float ao        = 0;
   vec3 Lo = vec3(0,0,0);
   float Geometry = 0;
   vec3 fresnel = vec3(0,0,0);
   float Distribution = 0;
   vec3 N = normalize(normal+normalM);
   vec3 V    = normalize((-camPos) - fragPos);
   vec3 F0 = vec3(0.04); 
   F0      = mix(F0, albedo, metallic);
   for(int i=0;i<numberOfLights;i++)
   {
      vec3 L  = normalize(lights[i].pos - fragPos);
   
      vec3 H = normalize(L + V);
      vec3 R = reflect(-L, N);  

      float distance    = length(lights[i].pos - fragPos);
       float attenuation = 1.0 / (distance * distance);
       vec3 radiance     = (lights[i].color*lights[i].intensity) * attenuation;
   
      
       vec3 F  = fresnelSchlick(max(dot(H, V), 0.0), F0);
       float NDF = DistributionGGX(N, H, roughness);       
       float G   = GeometrySmith(N, V, L, roughness); 
       vec3 numerator    = NDF * G * F;
       float denominator = 4.0 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0) + 0.0001 ;
       vec3 specular     = numerator / denominator;  
       vec3 kS = F;
       vec3 kD = vec3(1.0) - kS;
     
       kD *= 1.0 - metallic;	
       float shadow =  ShadowCalculation(fragPos,lights[i].pos);  
       Geometry+= G;
       fresnel+=F ;
       Distribution+=NDF;
       float NdotL = max(dot(N, L), 0.0);        
       Lo += ((kD * albedo / PI + specular) * radiance * NdotL)* (1.0 - shadow);

   }
 
   vec3 kS = fresnelSchlickRoughness(max(dot(N, V), 0.0), F0, roughness); 
   vec3 kD = 1.0 - kS;
   kD *= 1.0 - metallic;	
   vec3 reflection =texture(material.cubeMap,NormalToEquirectangularUv(reflect(V, N))).rgb;  
   vec3 irradiance =texture(material.cubeMap,NormalToEquirectangularUv(N)).rgb;
   vec3 diffuse = irradiance*albedo;
   vec3 ambient = (kD * diffuse) * ao;
   vec3 color = (ambient + Lo + reflection * kS);
   color = color / (color + vec3(1.0));
   color = pow(color, vec3(1.0/2.2));  
   //Debug
   color = mix(color, vec3(Geometry, Geometry , Geometry),debug.geometry);
   color = mix(color,fresnel,debug.fresnel);
   color = mix(color, vec3(Distribution, Distribution, Distribution),debug.distribution);

   fragColor = vec4(color, 1.0);
   
}
