#version 460 core

in vec3 vLocalPos;

uniform vec3 uLightDir;
uniform float uLightIntensity;

out vec4 FragColor;

void main()
{
	vec3 lightDir = normalize(-uLightDir);

	vec3 viewDir = normalize(vLocalPos);

	float sunDot  = dot(viewDir, lightDir);
	float sunDisc = smoothstep(0.9985, 0.9995, sunDot);
	vec3 sunColor = vec3(1.0, 0.95, 0.7);

	vec3 nightColor = vec3(0.01, 0.01, 0.05);
	vec3 dayColor = vec3(0.4, 0.7, 1.0);
	vec3 skyColor = mix(nightColor, dayColor, uLightIntensity);
	skyColor = mix(skyColor, sunColor, sunDisc);

	FragColor = vec4(skyColor, 1.0);
}
