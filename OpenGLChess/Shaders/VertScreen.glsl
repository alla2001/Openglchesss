#version 330 core
layout (location = 0) in vec3 aPos;


out vec3 fragDir;


uniform mat4 view;
uniform mat4 projection;

uniform vec3 camPos;


void main()
{

   vec4 screenPos =vec4( (aPos.x-0.5f)*2,(aPos.y-0.5f)*2,0.9999f, 1.0);
 
   fragDir = normalize(vec3(inverse(view)*inverse(projection) * (screenPos)));
   gl_Position = screenPos;
}