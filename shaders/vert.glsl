#version 410 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 vertexColors;
layout(location = 2) in vec2 texCoord;

uniform mat4 u_ModelMatrix;
uniform mat4 u_ViewMatrix;
uniform mat4 u_Projection;
uniform float u_Time;

out vec3 v_vertexColors;
out vec2 v_texCoord;

void main()
{
    v_vertexColors = vertexColors;
    v_texCoord = texCoord;
    vec3 pulsed = vec3(position.x, position.y + 0.02 * sin(u_Time * 4.0), position.z);
    gl_Position = u_Projection * u_ViewMatrix * u_ModelMatrix * vec4(pulsed, 1.0);
}