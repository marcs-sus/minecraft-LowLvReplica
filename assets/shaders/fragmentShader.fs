#version 330 core
out vec4 FragColor;

in vec2 TexCoord;

uniform sampler2D grassBlockTexture;

void main() {
    FragColor = texture(grassBlockTexture, TexCoord);
}