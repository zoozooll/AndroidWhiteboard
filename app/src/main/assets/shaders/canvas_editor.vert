#version 320 es

layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec2 aTexCoor;

out vec2 vTexCoor;

void main() {
    gl_Position = vec4(aPosition, 1.0);
    vTexCoor = aTexCoor;
}
