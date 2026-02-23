#version 460 core

in vec3 vNormal;
in vec2 vUV;

out vec4 FragColor;

void main()
{
    float variation = fract(float(gl_PrimitiveID) * 0.618033988749895);
	float brightness = 0.7 + variation * 0.6;

	vec3 tmp_color = vec3(0.5, 0.5, 0.5) * brightness;
	tmp_color = clamp(tmp_color, 0.0, 1.0);
	vec4 grey_color = vec4(tmp_color, 0.0);

	FragColor = grey_color;
}