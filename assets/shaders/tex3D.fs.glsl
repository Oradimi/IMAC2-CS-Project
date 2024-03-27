#version 330

uniform sampler2D uTexture;

in vec3 vUV;

out vec4 fFragColor;

void main()
{
    vec4 textureColor = texture(uTexture, vUV.xy);
    fFragColor = vec4(textureColor.xyz, 1.);
}
