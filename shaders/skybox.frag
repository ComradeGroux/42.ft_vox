#version 460 core

in vec3 vLocalPos;

out vec4 FragColor;

void main()
{
	vec3 lightDir = normalize(vec3(0.8, 1.0, 0.6));

	vec3 viewDir = normalize(vLocalPos);

	vec3 zenithColor = vec3(0.10, 0.40, 0.80);
	vec3 horizonColor = vec3(0.80, 0.65, 0.50);
	vec3 groundColor = vec3(0.15, 0.10, 0.08);

	float horizon = smoothstep(-0.1, 0.05, viewDir.y);
	vec3 skyColor = mix(horizonColor, zenithColor, horizon);

	float sunDot  = dot(viewDir, lightDir);
	float sunDisc = smoothstep(0.9985, 0.9995, sunDot);
	vec3  sunColor = vec3(1.0, 0.95, 0.7);
	skyColor = mix(skyColor, sunColor, sunDisc);

	FragColor = vec4(skyColor, 1.0);
}
