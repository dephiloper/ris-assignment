#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;

out vec2 texCoord;
out vec3 cameraPos;
out vec3 worldPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() {
    // read multiplication from right to left
    gl_Position = projection * view * model * vec4(aPos, 1.0);
    texCoord = vec2(aTexCoord.x, aTexCoord.y);
    cameraPos = (view * model * vec4(aPos, 1)).xyz;
    worldPos = (model * vec4(aPos, 1)).xyz;
}