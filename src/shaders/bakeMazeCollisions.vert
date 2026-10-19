#version 300 es
precision highp float;

void main() {
  float x = float((gl_VertexID & 1) << 2);
  float y = float((gl_VertexID & 2) << 1);

  gl_Position = vec4(x - 1.0f, y - 1.0f, 0.0f, 1.0f);
}
