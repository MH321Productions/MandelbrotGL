#version 300 es
precision highp float;

layout(location = 0) in vec2 position;

void main() {
    gl_Position = vec4(position, 0, 1);
}