#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D textureOne;

void main() {
  FragColor = texture(textureOne, TexCoords);
} 