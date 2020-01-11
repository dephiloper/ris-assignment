#version 330 core
out vec4 FragColor;

in vec2 texCoord;
in vec3 cameraPos;
in vec3 worldPos;

uniform vec4 laserColor;
uniform sampler2D texture0;
uniform float tileSize;
uniform int isLaser;

void main() {
    float d = length(cameraPos);
    vec4 textureColor = texture(texture0, texCoord);
    vec4 renderColor = mix(textureColor, laserColor, isLaser);
    FragColor = mix(renderColor, vec4(0.2f, 0.3f, 0.3f, 1.0f), clamp((d / tileSize) - .5, 0, .5) * 2); //vec4(1.0, 0.0, 0.0, 1.0);
}