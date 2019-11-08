#version 330 core
out vec4 FragColor;

in vec3 ourColor;
in vec2 TexCoord;

/*
 * The fragment shader should also have access to the texture object, but how do we pass the texture object
 * to the fragment shader? GLSL has a built-in data-type for texture objects called a sampler that takes as
 * a postfix the texture type we want e.g. sampler1D, sampler3D or in our case sampler2D. We can then add a
 * texture to the fragment shader by simply declaring a uniform sampler2D that we later assign our texture to.
*/
uniform sampler2D texture0;
uniform sampler2D texture1;
uniform float mixValue;

void main() {
    /* To sample the color of a texture we use GLSL's built-in texture function that takes as its first argument
     * a texture sampler and as its second argument the corresponding texture coordinate. The texture function then
     * samples the corresponding color value using the texture parameters we set earlier. The output of this fragment
     * shader is then the (filtered) color of the texture at the (interpolated) texture coordinate.
     */
    FragColor = mix(texture(texture0, TexCoord), texture(texture1, vec2(1 - TexCoord.x, TexCoord.y)), mixValue); //* vec4(ourColor, 1.0);
}
