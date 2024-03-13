#version 330

uniform sampler2D uEarthTexture;
uniform sampler2D uCloudTexture;

in vec3 vUV;

out vec4 fFragColor;

void main()
{
    vec4 textureColorEarth = texture(uEarthTexture, vUV.xy);
    vec4 textureColorCloud = texture(uCloudTexture, vUV.xy);
    fFragColor = vec4(textureColorEarth.xyz + textureColorCloud.xyz, 1.);
}
