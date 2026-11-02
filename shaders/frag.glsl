#version 410 core

in vec3 v_vertexColors;
in vec2 v_texCoord;
uniform sampler2D u_Texture;
out vec4 color;
void main()
{
    color = texture(u_Texture, v_texCoord);
} 