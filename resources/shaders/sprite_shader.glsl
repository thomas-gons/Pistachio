#shader vertex
#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aTexCoord;

uniform float x_dir;
uniform float y_dir;

out vec2 TexCoord;

void main()
{
	vec2 offset = vec2(x_dir, y_dir);
	gl_Position = vec4(aPos.xy + offset, aPos.z, 1.0);
	TexCoord = vec2(aTexCoord.x, aTexCoord.y);
}

#shader fragment
#version 330
out vec4 FragColor;

in vec3 ourColor;
in vec2 TexCoord;

// texture sampler
uniform sampler2D texture1;

void main()
{
	FragColor = texture(texture1, TexCoord);
}