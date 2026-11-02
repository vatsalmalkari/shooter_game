#version 410 core

in vec3 v_vertexColors;
in vec2 v_texCoord;
uniform sampler2D u_Texture;
uniform float u_Flash;
out vec4 color;

void main()
{
    vec4 texColor = texture(u_Texture, v_texCoord);
    color = mix(texColor, vec4(1.0), u_Flash);
}