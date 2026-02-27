#version 460 core

in vec3 vNormal;
in vec2 vUV;

uniform vec3 uLightDir;
uniform float uLightIntensity;
uniform sampler2D uAtlas;

out vec4 FragColor;

void main()
{
	vec4 texColor = texture(uAtlas, vUV);

	vec3 lightDir = normalize(-uLightDir);
	float diff	  = max(dot(vNormal, lightDir), 0.0) * uLightIntensity;
	float ambient = 0.3;

	float light	  = ambient + (1.0 - ambient) * diff;

	FragColor = vec4(texColor.rgb * light, texColor.a);
}
