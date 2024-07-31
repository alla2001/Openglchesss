#version 330 core
out vec4 fragColor;


in vec3 fragDir;


uniform sampler2D cubemap;

const float PI = 3.14159265359;

vec2 NormalToEquirectangularUv(vec3 normal)
{
    float phi = acos(-normal.y);
    float theta = atan(normal.z, normal.x) + PI;
    vec2 uv = vec2(theta / (2*PI), -phi / PI);
    return uv;
}

void main()
{

   fragColor = vec4(texture(cubemap, NormalToEquirectangularUv(normalize(fragDir))).rgb, 1.0);
   
}
