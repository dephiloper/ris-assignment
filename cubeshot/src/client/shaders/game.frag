#version 330 core
out vec4 FragColor;

in vec3 color;
in vec2 texCoord;
in vec3 cameraPos;
in vec3 worldPos;

uniform vec4 laserColor;
uniform sampler2D texture0;
uniform float tileSize;
uniform int isLaser;
uniform int useColor;

void main() {
    float dist = length(cameraPos);
    vec4 textureColor = mix(texture(texture0, texCoord), vec4(color, 1.0f), useColor);
    vec4 renderColor = mix(textureColor, laserColor, isLaser);
    FragColor = mix(renderColor, vec4(0.2f, 0.3f, 0.3f, 1.0f), clamp((dist / tileSize) - .5, 0, .5) * 2);
}