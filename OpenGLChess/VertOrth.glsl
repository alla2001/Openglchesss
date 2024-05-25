#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNorm;
layout (location = 2) in vec2 aUV;

out vec3 normal;
out vec3 fragPos;
out vec3 viewDir;
out vec2 TexCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;


void main()
{
   gl_Position =  projection* view * model * vec4(aPos.x,aPos.y,aPos.z, 1.0);
   fragPos = vec3(model * vec4(aPos, 1.0));
   normal = mat3(transpose(inverse(model)))*aNorm;
   viewDir = vec3(inverse(view)*inverse(projection)* vec4(0,0,0,0));
   TexCoords=aUV;
}