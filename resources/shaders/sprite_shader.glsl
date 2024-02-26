#shader vertex
#version 330 core
layout (location = 0) in vec2 Position;
layout (location = 1) in uint QuadId;

#define MAX_QUADS 1000

uniform QuadInfo {
	vec2 basePosition[MAX_QUADS];
	vec2 widthHeight[MAX_QUADS];
	vec2 texCoords[MAX_QUADS];
	vec2 texWidthHeight[MAX_QUADS];
};

out vec2 TexCoord;

void main() {
	vec3 basePosition = vec3(basePosition[QuadId], 0.5);
	vec2 widthHeight = Position * widthHeight[QuadId];
	vec3 newPosition = basePosition + vec3(widthHeight, 0.0);
	gl_Position = vec4(newPosition, 1.0);

	vec2 baseTexCoord = texCoords[QuadId];
	vec2 texWidthHeight = Position * texWidthHeight[QuadId];
	TexCoord = baseTexCoord + texWidthHeight;
}


#shader fragment
#version 330

layout(location = 0) out vec4 FragColor;

in vec2 TexCoord;

// texture sampler
uniform sampler2D texture1;


void main()
{
	FragColor = texture2D(texture1, TexCoord);

	if (FragColor == vec4(0.0)) {
		discard;
	}
}