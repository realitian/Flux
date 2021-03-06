#version 150 core

uniform sampler2D sources[8];
uniform int sourceCount;

in vec3 pass_position;
in vec2 pass_texCoords;

out vec4 fragColor;

void main() {
    vec3 color = vec3(0, 0, 0);
    float weight = 1.0f / sourceCount;
    for (int i = 0; i < sourceCount; i++) {
        color += texture(sources[i], pass_texCoords).rgb * weight;
    }
    
    fragColor = vec4(color, 1);
}
