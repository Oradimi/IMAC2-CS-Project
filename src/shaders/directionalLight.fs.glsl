#version 330

uniform sampler2D uTexture;
uniform vec3 uKd;
uniform vec3 uKs;
uniform float uShininess;
uniform vec3 uLightDir_vs;
uniform vec3 uLightIntensity;

in vec3 vUV;
in vec3 vPosition_vs;
in vec3 vNormal_vs;

out vec4 fFragColor;

vec3 blinnPhong() {
    return uLightIntensity 
    * (uKd * (dot(normalize(uLightDir_vs), normalize(vNormal_vs))) + uKs
    * pow(dot(normalize((normalize(-vPosition_vs)
    + normalize(uLightDir_vs)) / 2.f), normalize(vNormal_vs)), uShininess));
}

void main()
{
    vec4 textureColor = texture(uTexture, vUV.xy);
    fFragColor = vec4(textureColor.xyz * blinnPhong(), 1.);
}