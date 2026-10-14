#version 300 es
precision highp float;
out vec2 TexCoords;

void main() {
  float x = float((gl_VertexID & 1) << 2);
  float y = float((gl_VertexID & 2) << 1);

  TexCoords = vec2(x * 0.5f, y * 0.5f);
  gl_Position = vec4(x - 1.0f, y - 1.0f, 0.0f, 1.0f);
}
