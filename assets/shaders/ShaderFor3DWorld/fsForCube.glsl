#version 460 core
out vec4 FragColor;

//in vec3 ourColor;
in vec2 TexCoord;

uniform sampler2D ourTexture;
uniform vec3 light_color;

void main()
{
    FragColor = texture(ourTexture, TexCoord) * vec4(light_color, 1.0);
}