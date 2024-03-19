#shader vertex
#version 330 core

struct QuadInfo {
	vec2 quad_xy;
	vec2 quad_wh;
	vec2 tex_xy;
	vec2 tex_wh;
};


layout (std430, binding = 0) buffer QuadBuffer {
	QuadInfo quadInfo[];
};


out vec2 TexCoord;

void main() {
	int vertexId = gl_VertexID / 6;
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