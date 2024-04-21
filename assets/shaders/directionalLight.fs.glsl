#version 330

uniform sampler2D uTexture;
uniform vec3 uKd;
uniform vec3 uKs;
uniform float uShininess;
uniform vec3 uLightDir_vs;
uniform vec3 uWorldLightIntensity;

in vec3 vUV;
in vec3 vPosition_vs;
in vec3 vNormal_vs;

out vec4 fFragColor;

vec3 blinnPhong() {
    vec3 nLightDir_vs = normalize(uLightDir_vs);
    vec3 nNormal_vs = normalize(vNormal_vs);
    vec3 nPosition_vs = normalize(-vPosition_vs);

    vec3 halfwayVector = normalize(nLightDir_vs + nPosition_vs);

    float diffuseTerm = max(dot(nLightDir_vs, nNormal_vs), 0.0);
    vec3 diffuseColor = uKd * diffuseTerm;

    float specularTerm = pow(max(dot(nNormal_vs, halfwayVector), 0.0), uShininess);
    vec3 specularColor = uKs * specularTerm;

    return uWorldLightIntensity * (diffuseColor + specularColor);
}

void main()
{
    vec4 textureColor = texture(uTexture, vUV.xy);
    fFragColor = vec4(textureColor.xyz * (blinnPhong() + 0.2), 1.); // ensure minimum of 20% lighting
}
