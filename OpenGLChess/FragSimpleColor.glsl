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
}; 
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
void main()
{
   vec3 albedo     = texture(material.albedoMap, TexCoords).rgb;
   // vec3 normal     = getNormalFromNormalMap();
   float metallic  = texture(material.metallicMap, TexCoords).r;
   float roughness = 1-texture(material.roughnessMap, TexCoords).r;
   float ao        = 1;
   vec3 Lo = vec3(0,0,0);
   float Geometry = 0;
   vec3 fresnel = vec3(0,0,0);
   float Distribution = 0;
   vec3 N = normalize(normal);
   vec3 V    = normalize((-camPos) - fragPos);
   for(int i=0;i<numberOfLights;i++)
   {
      vec3 L  = normalize(lights[i].pos - fragPos);
   
      vec3 H = normalize(L + V);
      vec3 R = reflect(-L, N);  

      float distance    = length(lights[i].pos - fragPos);
       float attenuation = 1.0 / (distance * distance);
       vec3 radiance     = (lights[i].color*lights[i].intensity) * attenuation;
   
       vec3 F0 = vec3(0.04); 
       F0      = mix(F0, albedo, metallic);
       vec3 F  = fresnelSchlick(max(dot(H, V), 0.0), F0);
       float NDF = DistributionGGX(N, H, roughness);       
       float G   = GeometrySmith(N, V, L, roughness); 
       vec3 numerator    = NDF * G * F;
       float denominator = 4.0 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0) + 0.0001 ;
       vec3 specular     = numerator / denominator;  
       vec3 kS = F;
       vec3 kD = vec3(1.0) - kS;
     
       kD *= 1.0 - metallic;	
       
       Geometry+= G;
       fresnel+=F ;
       Distribution+=NDF;
       float NdotL = max(dot(N, L), 0.0);        
       Lo += (kD * albedo / PI + specular) * radiance * NdotL;
      
   }
   vec3 ambient = vec3(0.03) * albedo * ao;
   vec3 color = ambient + Lo;
   color = color / (color + vec3(1.0));
   color = pow(color, vec3(1.0/2.2));  
   //Debug
   color = mix(color, vec3(Geometry, Geometry , Geometry),debug.geometry);
   color = mix(color,fresnel,debug.fresnel);
   color = mix(color, vec3(Distribution, Distribution, Distribution),debug.distribution);

   fragColor = vec4(color , 1.0);
   
}
