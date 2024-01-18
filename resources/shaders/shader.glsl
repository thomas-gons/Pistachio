#shader vertex
#version 330 core
layout (location = 0) in vec2 position;
layout (location = 1) in vec2 inTexCoord;

out vec2 texCoord;
void main(){
    texCoord = inTexCoord;
    gl_Position = vec4(position.x, position.y, 0.0f, 1.0f);
}


#shader fragment
#version 330 core
out vec4 FragColor;

uniform vec2 iResolution;
uniform float iTime;

vec3 palette (float t) {
    vec3 a = vec3(0.5, 0.5, 0.5);
    vec3 b = vec3(0.5, 0.5, 0.5);
    vec3 c = vec3(1.0, 1.0, 1.0);
    vec3 d = vec3(0.243, 0.416, 0.557);
    return a + b*cos( 6.28318*(c*t+d) );
}


void main()
{
    vec2 uv = (gl_FragCoord.xy * 2.0 - iResolution) / iResolution.y;
    vec2 uv0 = uv;
    vec3 finalColor = vec3(0.0);

    for (float i = 0.0; i < 4.0; i++) {
        uv = fract(uv * 1.5) - 0.5;
        float d = length(uv) * exp(-length(uv0));

        vec3 col = palette(length(uv0) + i*.4 + iTime*.4);

        d = sin(d*8. + iTime)/8;
        d = abs(d);

        d = pow(0.01 / d, 1.2);

        finalColor += col * d;
    }


    FragColor = vec4(finalColor, 1.0);
}