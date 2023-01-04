#version 320 es
precision mediump float;

in vec2 vTexCoor;
uniform sampler2D canvasTexture;
uniform sampler2D prevTexture;
out vec4 fragColor;

void main() {
    vec4 canvasColor = texture(canvasTexture, vec2(vTexCoor.s, 1. - vTexCoor.t));
    vec4 prevColor = texture(prevTexture, vTexCoor);
    fragColor = canvasColor * canvasColor.a + prevColor * (1. - canvasColor.a);
}
