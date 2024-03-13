#version 330

uniform sampler2D uEarthTexture;
uniform sampler2D uCloudTexture;
uniform vec3 uKd;
uniform vec3 uKs;
uniform float uShininess;
uniform vec3 uLightPos_vs;
uniform vec3 uLightIntensity;
uniform float uTime;

in vec3 vUV;
in vec3 vPosition_vs;
in vec3 vNormal_vs;

out vec4 fFragColor;

vec3 blinnPhong() {
    float d = distance(uLightPos_vs, vPosition_vs);
    return (uLightIntensity / (d * d)) 
    * (uKd * (dot(normalize(uLightPos_vs - vPosition_vs), normalize(vNormal_vs))) + uKs
    * pow(dot(normalize((normalize(-vPosition_vs)
    + normalize(uLightPos_vs - vPosition_vs)) / 2.f), normalize(vNormal_vs)), uShininess));
}

void main()
{
    vec4 textureColorEarth = texture(uEarthTexture, vUV.xy);
    vec4 textureColorCloud = texture(uCloudTexture, vec2(0.05f * uTime + vUV.x, vUV.y));
    fFragColor = vec4((textureColorEarth.xyz + textureColorCloud.xyz) * blinnPhong(), 1.);
}