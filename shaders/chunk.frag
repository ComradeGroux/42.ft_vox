#version 460 core

in vec3 vNormal;
in vec2 vUV;

out vec4 FragColor;

uniform sampler2D uAtlas;

void main()
{
	vec4 texColor = texture(uAtlas, vUV);

	vec3 lightDir = normalize(vec3(0.8, 1.0, 0.6));
	float diff	  = max(dot(vNormal, lightDir), 0.0);
	float ambient = 0.3;
	float light	  = ambient + (1.0 - ambient) * diff;

	FragColor = vec4(texColor.rgb * light, texColor.a);
}
