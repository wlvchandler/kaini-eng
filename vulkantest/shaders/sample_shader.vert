#version 450

layout (location = 0) in vec2 position; // takes its value from a vertex buffer

void main() {
    gl_Position = vec4(position, 0.0, 1.0);
}