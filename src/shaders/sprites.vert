#version 300 es
precision highp float;

layout(location = 0) in vec2 aPos;
out vec2 uv;
uniform mat3 transform;

void main() {
  uv = aPos * vec2(0.5) + vec2(0.5);
  vec2 transformedPos = vec2((vec3(aPos, 0) * transform).xy);
  gl_Position = vec4(transformedPos, 0.0, 1.0);
}
