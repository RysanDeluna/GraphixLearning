#version 460 core
layout (location = 0) in vec3 aPos;   // the position variable has attribute position 0

uniform mat4 model;
uniform mat4 view;
uniform mat4 projc;

void main()
{
    gl_Position = projc * view * model * vec4(aPos, 1.0);
}