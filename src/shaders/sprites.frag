#version 300 es
precision mediump float;

out vec4 FragColor;
in vec2 uv;
uniform sampler2D tex;

void main() {
  FragColor = texture(tex, uv);
}
